#include "RCD_header.h"
//Client
vector<string> Operations_info;
mutex Operations_info_mutex;
ifstream raw_Data;
string default_ip = "0.0.0.0";
/*   
        ___  ___       _      _                      
        |  \/  |      | |    | |        powered by:               
        | .  . |  ___ | |  __| | ___ _ __ ___  _ ___  
        | |\/| | / _ \| | / _` |/ _ \ '__/ _ \| '_  \ 
        | |  | || (_)|| || (_| |  __/ | | (_) | | | |
        \_|  |_/\___/ |_|\\__,_|\___|_| \___./|_| |_|


*/
void Usage(){

             cout<<"\n\n       *********   --<READ ME>--    ********** \n";
             cout<<"      |---------------------------------------|\n";
             cout<<"      |    RCD -> Robot-Code_Deploy           |\n";
             cout<<"      |    This is a simple TCP c/s program,  |\n";
             cout<<"      |    to remote execute code for         |\n";
             cout<<"      |    'Dumbo the Robot', it depends      |\n";
             cout<<"      |    on (libczmq-dev) library           |\n";
             cout<<"      |    also known as <czmq.h>             |\n";
             cout<<"      |    So make sure it's installed.       |\n";
             cout<<"      |                                       |\n";
             cout<<"      |                  __                   |\n";
             cout<<"      |                 /__\\                  |\n";
             cout<<"      |                //  \\\\                 |\n";
             cout<<"      |              // v0.1 \\\\               |\n";
             cout<<"      |            //          \\\\             |\n";
             cout<<"      |          //|NOT FINISHED|\\\\           |\n";
             cout<<"      |                                       |\n";
             cout<<"      |                                       |\n";
             cout<<"      |      (Use 'zsock man' for info)       |\n";
             cout<<"      |         Compile with Makefile         |\n";
             cout<<"      |_______________________________________|\n";
             cout<<"           ||*********/Usage/*********||\n";
             cout<<"           ||                         ||\n";
             cout<<"           ||1. tasks-(run/save/close)||\n";
             cout<<"           ||                         ||\n";
             cout<<"           ||2. file to be deployed.  ||\n";
             cout<<"           ||                         ||\n";
             cout<<"           ||*************************||\n";
}

void Generate_IP(){
    cout<<"Please enter local/web addr for Robot\n";
    cout<<"\t\t...->";
    cin>>default_ip;
    default_ip.assign("tcp://"+default_ip":65535");
    system("clear");
}


bool Language_check(const string file_name){
    //fix this
    cout<<file_name<<endl;
    if(file_name.size() <3){return false;}

    string PL; size_t dot = file_name.find(".");
    if(dot == string::npos){ return false;}
    else{

        if(dot+1 > file_name.size()){return false;}
        for(int i = dot+1; i < file_name.size(); i++) {PL.push_back(file_name[i]);}

        if(PL == "c" || PL == "cpp" || PL == "py"){ return true;}
        else{cerr<<"\n[ERR] - Supported languages are: C/C++/Python (3)\n"; return false;}
        
    }
}



bool Program_Usage(const int& argc,  char** argv){
   
    if(argc != 3){
       cout<<"\t-> Usage is: ./Client + run/save + file_name\n\t-> Or: ./RCD_client -help"<<endl;
       return false;
    }  
    else if(strcmp(argv[1],"-help") == true){  
        Usage();
        return false;
    }
    else
    {
      return true;
    }
}


// Can be optimized 
bool Input(const int& argc, char** argv)
{  
    bool Openning_state_flags[3] = {false, false, false};
    
    string File_content;
    unique_lock<mutex> guard(Operations_info_mutex);
    
    
    Program_Usage(argc, argv) == true ? Openning_state_flags[0] = true : Openning_state_flags[0] = false;
    if(Openning_state_flags[0] == false){guard.unlock(); return false;}
    
    
    Openning_state_flags[1] = Language_check(argv[2]);
    if(Openning_state_flags[1] == false){guard.unlock(); Openning_state_flags[1] = false;} 

    Operations_info.push_back(argv[1]); //exec-state
    Operations_info.push_back(argv[2]); //File_Name


    raw_Data.open(Operations_info[1], ios::in);
    if(!raw_Data.is_open())
    {cerr<<"[ERR] - File could not be opened\n\n"; guard.unlock(); Openning_state_flags[2] = false;}
    
        else if(raw_Data.peek() == ifstream::traits_type::eof())
        {cerr<<"[ERR] - File: '"<<argv[2]<<"' is empty\n\n"; guard.unlock(); Openning_state_flags[2] = false;}

            else{
                string File_line;
                while(getline(raw_Data, File_line)){
                      File_content.append(File_line);
                      
                }
                Openning_state_flags[2] = true;
            }
    raw_Data.close();

    if(Openning_state_flags[0]==false ||
       Openning_state_flags[1]==false ||
       Openning_state_flags[2]==false)
             {return false;}

    string Server_task = argv[1];
    if(Server_task == "-run" || Server_task =="-save"|| Server_task == "-close" || Server_task == "-make"){
        Operations_info.push_back(File_content);
        return true;
    }
    else {return false;} 
}




int main(int argc, char** argv){
    Generate_IP();
     
     future<bool> code_dispatch = async(std::launch::async,Input,argc, argv);    
     future<bool> net_check = async(std::launch::async,Robot_Connection_Available, default_ip);
     
     bool Network_state = net_check.get();
     bool Input_state = code_dispatch.get();

     
     if(Network_state == false || Input_state == false)
     {
         Usage();
         Network_state == false ? cerr<<"\n[ERR] - Server not found, due to networking issue\n" : cout<<"\nConnection Established <3\n";
         Input_state == false ? cerr<<"\n[ERR] - Corrupted file, please reconfigure\n" : cout<<"\nFile integrity is: [OK]\n";
         exit(1);
     }
     
      Operations_info[0] = encryption_layer1(Operations_info[0]); 
      /* Operations_info[0] -> task */
      Operations_info[1] = encryption_layer1(Operations_info[1]);       
      /* Operations_info[1] -> file name */
      Operations_info[2] = encryption_layer1(Operations_info[2]);
      /* Operations_info[2] -> file content */
      
      
      

      Operations_info[0] = encrypt_layer2(Operations_info[0]);
      Operations_info[1] = encrypt_layer2(Operations_info[1]);
      Operations_info[2] = encrypt_layer2(Operations_info[2]);

        
    //future<bool> Monitoring = async(std::launch::async, Robot_monitoring);

    if(Robot_Code_Deploy(Operations_info, default_ip) != false){cout<<"Code Deployed!\n";}
    else{cout<<"[System]::Something went wrong :/\n\n";}

    //bool Conclusion = Monitoring.get();

   return 0;
}    
    