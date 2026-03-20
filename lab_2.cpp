#include <iostream>
#include<vector>
#include <fstream>
#include <ctime>
#include<chrono>
#include<Eigen/Dense>
#include<string>
#include<omp.h>

std::vector<std::vector<double>> matrix_multiplication(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B) {
	int n = A.size();
	std::vector<std::vector<double>> C(n, std::vector<double>(n, 0.0));
#pragma omp parallel for collapse(2)
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			double sum = 0.0;
			for (int k = 0; k < n; ++k) {
				sum += A[i][k] * B[k][j];
			}
			C[i][j] = sum;
		}
	}
	return C;
}



std::vector<std::vector<double>> read_file(std::string input_file) {
	std::ifstream inp(input_file);
	int n = 0;
	inp >> n;
	std::vector<std::vector<double>>A(n, std::vector<double>(n));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			inp >> A[i][j];
		}
	}
	return A;
}

void write_file(std::string output_file, const std::vector<std::vector<double>>& C) {
	std::ofstream out(output_file);
	int n = C.size();
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (j > 0) out << " ";
			out << C[i][j];
		}
		out << "\n";
	}
}

void recording_statistics(std::chrono::duration<double> lead_time,
	const std::vector<std::vector<double>>& C,
	std::string output_file,
	int num_threads) {
	int n = C.size();
	long long operations = 2LL * n * n * n;


	bool write_header = false;
	std::ifstream check(output_file, std::ios::binary);
	if (!check.good() || check.peek() == std::ifstream::traits_type::eof()) {
		write_header = true; 
	}
	check.close();

	std::ofstream out(output_file, std::ios::app);
	if (!out.is_open()) {
		std::cerr << "Ошибка открытия файла: " << output_file << "\n";
		return;
	}


	if (write_header) {
		out << "Size,Num_threads,Time_Seconds,Operations\n";
	}
	out << n << ","
		<< num_threads << ","
		<< lead_time.count() << ","
		<< operations << "\n";

	out.flush();
	out.close();
}

	bool verify_with_eigen(const std::vector<std::vector<double>>&A, const std::vector<std::vector<double>>&B, const std::vector<std::vector<double>>&C) {
		int n = A.size();
		Eigen::MatrixXd eigenA(n, n);
		Eigen::MatrixXd eigenB(n, n);
		Eigen::MatrixXd eigenC_cpp(n, n);

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				eigenA(i, j) = A[i][j];
				eigenB(i, j) = B[i][j];
				eigenC_cpp(i, j) = C[i][j];
			}
		}
		Eigen::MatrixXd eigenC = eigenA * eigenB;
		double max_diff = (eigenC - eigenC_cpp).cwiseAbs().maxCoeff();
		return max_diff < 1e-10;
	}


int main() {
	int num_threads = 12;
	auto A = read_file("matrix_A.txt");
	auto B = read_file("matrix_B.txt");
	if (A.size() != B.size() || A[0].size() != B.size()) {
		std::cerr << "Ошибка: несовместимые размеры матриц!\n";
		return 1;
	}
	omp_set_num_threads(num_threads);
	auto start = std::chrono::steady_clock::now();
	auto C = matrix_multiplication(A, B);
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> lead_time = end - start;
	std::cout << verify_with_eigen(A, B, C);
	write_file("result_matrix.txt", C);
	recording_statistics(lead_time, C, "result_statistic.csv",num_threads);
	return 0;
}