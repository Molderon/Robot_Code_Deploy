#include "RCD_server.h"
//Servent
bool RCD_operational = true;
mutex mute1, mute2;
/*   
        ___  ___       _      _                      
        |  \/  |      | |    | |        powered by:               
        | .  . |  ___ | |  __| | ___ _ __ ___  _ ___  
        | |\/| | / _ \| | / _` |/ _ \ '__/ _ \| '_  \ 
        | |  | || (_)|| || (_| |  __/ | | (_) | | | |
        \_|  |_/\___/ |_|\\__,_|\___|_| \___./|_| |_|


*/

void order_instructions(vector<string>& Instructions, const string& raw_packet){
        stringstream In_line(raw_packet);
        string index;      
        while(In_line >> index){
                if(index == "-close") RCD_operational = false;
                Instructions.push_back(index);
        }
}

void Decryption(vector<string>&Instructions){
            unique_lock<mutex> lock1(mute1);
            //Task
            Instructions[1] = decryption_layer1(decrypt_layer2(Instructions[1].c_str()));
            //File Name
            Instructions[2] = decryption_layer1(decrypt_layer2(Instructions[2].c_str()));
            lock1.unlock();
}


class Code_Initializer{
    
    protected:
        vector<string> Instructions;
        string raw_packet, reply;
        ofstream File; 
        unique_ptr<string>Program_Source;
        friend void order_instructions(vector<string>& Instructions, const string& raw_packet);
        friend void Decryption(vector<string>&Instuctions);

    public:
    Code_Initializer(pair<bool, zsock_t*>& Master_user){

        raw_packet = zstr_recv(Master_user.second);
        order_instructions(Instructions, raw_packet);
    

        
        if(!Instructions.empty()){
            Program_Source = make_unique<string>();
            (*Program_Source).resize(stoi(Instructions[0]), '0');
        }
            else{
                cerr<<"[System]::Unexpectred behaviour\n";
                exit(1);    
            }
            
        thread t1(Decryption, ref(Instructions));
        zstr_send(Master_user.second, recall);

        (*Program_Source) = zstr_recv(Master_user.second);  
        (*Program_Source) = decrypt_layer2((*Program_Source).c_str());
        (*Program_Source).assign(decryption_layer1((*Program_Source).c_str()));

        if(t1.joinable()){t1.join();}
        
        //---------------Troubled Code----------------------//

        Create_File(Instructions, File, Program_Source, Master_user) == true ? 
        this->reply = "Code Deployed!" :
        this->reply = "[System]:: File could not be created";
        
        zstr_send(Master_user.second, reply.c_str());
        zsock_destroy(&Master_user.second);

    }
    


    bool Execute(){
        if(this->Instructions[1] != "-run"){
            return false;
        }
        string pl;
        char dot = '.';
        size_t dot_index = Instructions[2].find(dot);
        
        if( dot_index!= string::npos){
            for(short unsigned int i = dot_index+1; i <Instructions[2].size(); ++i){
                pl.push_back(Instructions[2][i]);
            }

            if(pl == "c"){
                string command = "gcc " + Instructions[2]+ " -o " + "test";
                system(command.c_str());
                this_thread::sleep_for(3s);
                system("./test");
                return true;
            }
            else if(pl == "cpp"){
                string command = "g++ " + Instructions[2]+ " -o " + "test";
                system(command.c_str());
                this_thread::sleep_for(3s);
                system("./test");
                return true;
            }
            else if(pl =="py"){
                string command = "python3 "+Instructions[2];
                system(command.c_str());
                return true;
            }
            else{
                // Wrong format
                return false;
            }


        }
        
        else{
            return false;
        } 
    }

        bool Create_File(vector<string>& Instructions, ofstream& File,
                         unique_ptr<string>& Program_Source,
                         pair<bool, zsock_t*>& Master_user)
        {
            
            File.open(Instructions[2].c_str(), ios::out);
            if(!File.is_open()){
                return false;
            }

            File << (*Program_Source);
            File.close();
            return true;
        }

};



int main(){

    while(RCD_operational){
        if(net_test() == false){ 
            cerr<<"\nNetworking Error\n";
            exit(1);
        }
        
        pair<bool, zsock_t*> Master_user = Authenticate_Master();
        if((Master_user.first == false) || (Master_user.second == nullptr)){exit(1);}

        Code_Initializer* handler = new Code_Initializer(Master_user);
    
        if(handler->Execute() == true){
            zstr_send(Master_user.second, "\n\tANAKIN DROP!\n");
        }
        delete handler;
        zsock_destroy(&Master_user.second);
    }

  return 0;
}