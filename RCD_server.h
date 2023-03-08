#pragma once

#include <iostream>
#include <thread>
#include <utility>
#include <mutex>
#include <vector>
#include <fstream>
#include <czmq.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <memory>
#include <array>
#include <set>
#include <deque>
// C - environment

#include <typeinfo>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using namespace std;

static int default_port = 65535;
const char recall[] = "Lost vox";

const char* create_addr(string* address);
bool net_test();
string encryption_layer1(string s);
std::string encrypt_layer2(std::string message);

pair<bool, zsock_t*> Authenticate_Master();

string decryption_layer1(string s);
string decrypt_layer2(string Robot_reply);