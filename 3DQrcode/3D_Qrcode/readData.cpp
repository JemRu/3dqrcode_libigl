
#include "readData.h"
#include <igl\file_dialog_open.h>
#include "qrcodeGenerator.h"
bool qrcode::readData(Eigen::MatrixXi & D)
{
	std::string input ="";
	input = igl::file_dialog_open();
	if (input != "") {
		readData(input, D);
		return true;
	}else
		return false;
}

int qrcode::readData(Eigen::MatrixXd & D)
{
	std::string input = "";
	input = igl::file_dialog_open();
	if (input != "")
		return readData(input, D);
	else
		return 0;
}

bool qrcode::readData(const std::string file, Eigen::MatrixXi & D)
{
	int cols, rows, n;
	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>R, G, B, A;
	unsigned char *data = stbi_load(file.c_str(), &cols, &rows, &n, 4);
	if (data == NULL) {
		return false;
	}
	R.resize(cols, rows);
	G.resize(cols, rows);
	B.resize(cols, rows);
	A.resize(cols, rows);
	D.resize(cols, rows);
	for (unsigned i = 0; i < rows; ++i) {
		for (unsigned j = 0; j < cols; ++j) {
			R(j, rows - 1 - i) = data[4 * (j + cols * i) + 0];
			G(j, rows - 1 - i) = data[4 * (j + cols * i) + 1];
			B(j, rows - 1 - i) = data[4 * (j + cols * i) + 2];
			A(j, rows - 1 - i) = data[4 * (j + cols * i) + 3];
		}
	}

	stbi_image_free(data);
	Eigen::MatrixXd temp = 0.3*R.cast<double>() + 0.59*G.cast<double>() + 0.11*B.cast<double>();
	for (int i = 0; i < temp.rows(); i++) {
		for (int j = 0; j < temp.cols(); j++) {
			D(i, j) = (temp(i, j) > 0) ? 1 : 0;
		}
	}
	R.resize(0,0);
	G.resize(0,0);
	B.resize(0,0);
	A.resize(0,0);
	return true;
}

int qrcode::readData(const std::string file, Eigen::MatrixXd & D)
{
	int mask = 0;
	int border = 1;
	int errColLvl = 0;
	int scale = 1;
	char text[1024];
	const char* input = file.c_str();
	FILE* in = fopen(input, "r");
	if (in == (FILE*)NULL) {
		printf("Cannot open the qrcode info file...");
		return 0;
	}
	else {
		fscanf(in, "%d %d %d %d\n", &errColLvl, &mask, &border,&scale);
		fscanf(in, "%s\n", &text);
		std::string str = text;
		switch (errColLvl)
		{
		case 0:
			qrCodeGenerator(str, qrcodegen::QrCode::Ecc::LOW, mask, border, D);
			break;
		case 1:
			qrCodeGenerator(str, qrcodegen::QrCode::Ecc::MEDIUM, mask, border, D);
			break;
		case 2:
			qrCodeGenerator(str, qrcodegen::QrCode::Ecc::QUARTILE, mask, border, D);
			break;
		case 3:
			qrCodeGenerator(str, qrcodegen::QrCode::Ecc::HIGH, mask, border, D);
			break;
		}
		return scale;
	}
	
}
