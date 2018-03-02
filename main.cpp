#include <iostream>
#include <fstream>
#include <sstream>

class matrix_t{
private:
	int ** data_;
	unsigned int rows_;
	unsigned int collumns_;

public:
	matrix_t() {
		data_ = nullptr;
		rows_ = 0;
		collumns_ = 0;
	}

	matrix_t(matrix_t const & matrix) {
		rows_ = matrix.rows_;
		collumns_ = matrix.collumns_;

		data_ = new int *[rows_];
		for (unsigned int i = 0; i < rows_; ++i) {
			data_[i] = new int[collumns_];
			for (unsigned int j = 0; j < collumns_; ++j) {
				data_[i][j] = matrix.data_[i][j];
			}
		}
	}

	matrix_t & operator = (matrix_t const & matrix) {
		if (this != &matrix) {
			for (unsigned int i = 0; i < rows_; ++i) {
				delete[] data_[i];
			}
			delete[] data_;

			rows_ = matrix.rows_;
			collumns_ = matrix.collumns_;
			data_ = new int *[rows_];
			for (unsigned int i = 0; i < rows_; ++i) {
				data_[i] = new int[collumns_];
			}
			for (unsigned int i = 0; i < rows_; ++i) {
				for (unsigned int j = 0; j < collumns_; ++j) {
					data_[i][j] = matrix.data_[i][j];
				}
			}
		}

		return *this;
	}

	matrix_t add(matrix_t const & other) const {

		if (rows_ == other.rows_ &&
			collumns_ == other.collumns_) {

			matrix_t result(other);

			for (unsigned int i = 0; i < rows_; ++i) {
				for (unsigned int j = 0; j < collumns_; ++j) {
					result.data_[i][j] = data_[i][j] + other.data_[i][j];
				}
			}
			return result;
		}
		else {
			error();
			exit(0);
		}
	}

	matrix_t sub(matrix_t const & other) const {
		if (rows_ == other.rows_ &&
			collumns_ == other.collumns_) {

			matrix_t result(other);

			for (unsigned int i = 0; i < rows_; ++i) {
				for (unsigned int j = 0; j < collumns_; ++j) {
					result.data_[i][j] = data_[i][j] - other.data_[i][j];
				}
			}
			return result;
		}
		else {
			error();
			exit(0);
		}
	}

	matrix_t mul(matrix_t const & other) const {
		matrix_t result;

		if (collumns_ == other.rows_) {
			result.rows_ = rows_;
			result.collumns_ = other.collumns_;
			
			result.data_ = new int *[rows_];
			for (int i = 0; i < rows_; ++i) {
				result.data_[i] = new int[other.collumns_];
			}


			for (int i = 0; i < rows_; ++i) {
				for (int j = 0; j < other.collumns_; ++j) {
					int summ = 0;
					for (int k = 0; k < other.rows_; ++k) {
						summ += data_[i][k] * other.data_[k][j];
					}
					result.data_[i][j] = summ;
				}
			}
		}
		else {
			error();
		}

		return result;
	}

	matrix_t trans() const {
		matrix_t result;

		result.collumns_ = rows_;
		result.rows_ = collumns_;
		result.data_ = new int *[result.rows_];
		for (unsigned int i = 0; i < collumns_; ++i) {
			result.data_[i] = new int [result.collumns_];
		}

		for (unsigned int i = 0; i < result.rows_; ++i) {
			for (int unsigned j = 0; j < result.collumns_; ++j) {
				result.data_[i][j] = data_[j][i];
			}
		}
		return result;
	}

	std::ifstream & read( std::ifstream & stream, const std::string fileName ) {
		stream.open(fileName.c_str());

		char symb;
		if (stream.is_open() &&
			stream >> rows_ &&
			stream >> symb && symb == ',' &&
			stream >> collumns_) {
			data_ = new int *[rows_];
			for (unsigned int i = 0; i < rows_; ++i) {
				data_[i] = new int[collumns_];
				for (unsigned int j = 0; j < collumns_; ++j) {
					stream >> data_[i][j];
				}
			}
		}
		else {
			error();
		}
		return stream;
	}

	std::ofstream & write( std::ofstream & stream, const std::string fileName ) const {
		stream.open(fileName.c_str());

		if (stream.is_open()) {
			for (unsigned int i = 0; i < rows_; ++i) {
				for (unsigned int j = 0; j < collumns_; ++j) {
					stream << data_[i][j] << '\t';
					if (j == collumns_ - 1) {
						stream << '\n';
					}
				}
			}
		}
		else {
			error();
		}
		return stream;
	}

	void error() const {
		std::cout << "An error has occured while reading input data__\n";
	}

	~matrix_t() {
		for (unsigned int i = 0; i < rows_; ++i) {
			delete[] data_[i];
		}
		delete[] data_;
	}
};


int main()
{
	matrix_t A;

	std::string line;
	std::getline(std::cin, line);

	std::istringstream stroka(line);
	std::string fileNameA;
	
	std::ifstream streamA;
	
	stroka >> fileNameA;
	char symb;
	if ((A.read(streamA, fileNameA)) && (stroka >> symb)) {
		if (symb == '+' || symb == '-' || symb == '*' ) {
			matrix_t B;
			matrix_t R;

			std::string fileNameB, fileNameR = "result.txt";
			std::ifstream streamB;
			stroka >> fileNameB;
			std::ofstream streamR;

			if (B.read(streamB, fileNameB)) {
	             
				if (symb == '+') {
					R = A.add(B);
					R.write(streamR, fileNameR);
				}
			else if (symb == '-') {
					R = A.sub(B);
					R.write(streamR, fileNameR);
				}
			else if (symb == '*') {
					R = A.mul(B);
					R.write(streamR, fileNameR);
				}
			}
			else {
				B.error();
			}
			streamB.close();
			streamR.close();
		}
		else if (symb == 'T') {
			matrix_t R;
			std::string fileNameR = "result.txt";
			std::ofstream streamR;
			
			R = A.trans();
			R.write(streamR, fileNameR);

			streamR.close();
		}
		else {
			A.error();
		}
	}
	else {
		A.error();
	}
	
	streamA.close();
	std::cin.get();
    return 0;
}
