#pragma once
#include <iostream>
#include <string>
#include <future>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <memory>
#include <iterator>
#include <functional>
#include <utility>
#include <fstream>
#include <czmq.h>
#include <set>
#include <math.h>
#include <deque>
#include <algorithm>
#include <tuple>
#include <array>

using namespace std;


const string prove_sypher ="I'm your master-recreator..."; // \m/

const char recall[] = "Lost vox";
bool Program_Usage(const int& argc,  char** argv);
bool Input(const int& argc, char** argv);

bool Robot_Code_Deploy(const vector<string>& Operations_info,const string& default_ip);
bool Robot_Connection_Available(const string& default_ip);
bool Robot_monitoring();

deque<string> Convert_Binary(const string& message);


string encryption_layer1(string s);
string decryption_layer1(string s);


std::string encrypt_layer2( std::string message);
std::string decrypt_layer2( std::string& Robot_reply);