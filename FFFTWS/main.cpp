
#include "include/masesk/EasySocket.hpp"
#include "include/fftw3.h"
#include <sstream>
#include <string>
#include <vector>
#include <iostream>






using namespace std;
using namespace masesk;

#define REAL 0
#define IMAG 1

int packetRecieved = 0;

EasySocket socketManager;

void tokenize(string s, std::vector<double>* result, string del = " " )
{
	
}

void handleData(const std::string& data) {
	

	std::vector<double> result;
	string del = " ";

	int start = 0;
	int end = data.find(del);
	while (end != -1) {


		std::string a = data.substr(start, end - start);


		result.push_back(stoi(a));

		start = end + del.size();
		end = data.find(del, start);
	}

	std::string a = data.substr(start, end - start);

	result.push_back(stoi(a));




	packetRecieved++;

	int size = result.size();


	std::cout <<"SIZE: " << size << std::endl;
	
	//TRANSFORM IT TO COMPLEX
	fftw_complex out[2048];
	fftw_plan plan = fftw_plan_dft_r2c_1d(2048, result.data(), out, FFTW_ESTIMATE);
	fftw_execute(plan);
	fftw_destroy_plan(plan);
	
	
	//TRANSFORM IT TO BACK
	double back[2048];

	fftw_plan plan2 = fftw_plan_dft_c2r_1d(2048, out, back, FFTW_ESTIMATE);

	fftw_execute(plan2);
	fftw_destroy_plan(plan2);

	socketManager.socketConnect("test1", "192.168.0.104", 8080);

	for (size_t i = 0; i < 2048; i++)
	{
		socketManager.socketSend("test1", std::to_string(back[i] / 2048 ));
	}
	
	socketManager.closeConnection("test1");

	//fftw_destroy_plan(plan);
	//fftw_destroy_plan(plan2);
	//fftw_cleanup();
}


void ThreadFunction()
{
	socketManager.socketListen("test", 8080, &handleData);
}

int main() {

	socketManager.socketListen("test", 8080, &handleData);
	
	
	return 0;
}

