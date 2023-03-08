#include "RCD_header.h"



bool Robot_Connection_Available(const string& default_ip){
        // request - responce
    bool server_established = false;
    string login = encryption_layer1(prove_sypher);
    
    zsock_t *requester = zsock_new(ZMQ_REQ);  
    zsock_connect(requester, default_ip.c_str());
    zstr_send(requester, login.c_str());

    sleep(1); 
    
    string Robot_reply = decryption_layer1(zstr_recv(requester));
    zsock_destroy(&requester);
    Robot_reply.pop_back();// let's say bugs become features :D
    
    Robot_reply == "Connection Approved" ? server_established = true : server_established = false;
    cout<<"\nMachine replayed with: ["<<Robot_reply<<"]"<<endl; 
    return server_established; 
}

 
 bool Robot_monitoring(){
  /*
  make a second channel to monitor robot behaviour
   thread one -> Operation status

   thread two-:
   - error line
   - Useful data
   - Control Log
   */
  return true;
 }


 bool Robot_Code_Deploy(const vector<string>& Operations_info, const string& default_ip)
 {
   bool Code_Deployed = false;
   string Robot_reply;
   const size_t Buffer_forsight = Operations_info[2].size();
   string Code_Deploy[2] = {"",""}; 
  
   Code_Deploy[0] =     to_string(Buffer_forsight)+  "\n"
                              + Operations_info[0]+  "\n"
                              + Operations_info[1];                     
   Code_Deploy[1] =            (Operations_info[2]);
   
   //socket && Dispatch
   zsock_t *Requester = zsock_new(ZMQ_REQ);
   zsock_connect(Requester, default_ip.c_str());
   zstr_send(Requester, Code_Deploy[0].c_str());

   if(strcmp(recall,zstr_recv(Requester)) == 1){
     zsock_destroy(&Requester);
     cout<<"[System]::Networking Error\n";
     return false;
   }
   
   zstr_send(Requester, Code_Deploy[1].c_str());
   std::this_thread::sleep_for(std::chrono::microseconds(400));
   Robot_reply = zstr_recv(Requester);

   //whrite a function to monitor the time duration for wait
   //handle bad code deploy
   
   if(Robot_reply.empty()){Code_Deployed = false;}
   else{Robot_reply = decryption_layer1(decrypt_layer2(Robot_reply));
        Robot_reply == "Code Deployed!" ? Code_Deployed = true : Code_Deployed = false;
    }
 
   zsock_destroy(&Requester);
   
   return Code_Deployed;
 }


  //Encryption algorithms



class Decrypted_unit {
  string result;
  int index, ordered;
public:
  Decrypted_unit(string  message){
    for (size_t i = 0; i < message.length(); i += 9) {
      string bin = message.substr(i, 8);
      index = message[i + 8] - '0';
       vector<string> rows(8);
      for (int k = 0; k < 8; k++) {
        for (int j = 0; j < 8; j++) {
          rows[j] = bin[j] + rows[j];
        }
        sort(rows.begin(), rows.end());
      }
      ordered = stoi(rows[index], nullptr, 2);
      this->result += (char)ordered;
    }
  }
  
  string get_decrypted() { return this->result; }
};




class Encrypted_unit {
private:
  multiset<string> Lex; array<string, 8>BiT_Permutations;
  vector<pair<int, string>> Originals; string Encrypted;

public:
  Encrypted_unit(deque<string>& Bianry_Message) {

    for (int index = 0; index < Bianry_Message.size(); ++index) {

      this->Originals.push_back(make_pair(index, Bianry_Message[index]));
      Bianry_Message[index] = Permutatons(BiT_Permutations, Bianry_Message, index, this->Lex);

    }
    for (unsigned short int a = 0; a < Bianry_Message.size(); a++) { this->Encrypted.append(Bianry_Message[a]); }
  }



protected:

  string Permutatons(array<string, 8>& BiT_Permutations,
    deque<string>& Binary_Message, const int& index, multiset<string>& Lex)
  {
    set<string>::iterator itr; string Solution; int search_iterations = 0, found_at = 0;
    string current_permute = Binary_Message[index], original_key = Binary_Message[index];

    for (unsigned short int x = 0; x < current_permute.size(); x++) {
      if (current_permute.front() == '0') {
        current_permute.erase(current_permute.begin() + 0);
        current_permute.push_back('0');

        BiT_Permutations[x] = current_permute;
      }

      else if (current_permute.front() == '1') {
        current_permute.erase(current_permute.begin() + 0);
        current_permute.push_back('1');

        BiT_Permutations[x] = current_permute;
      }
    }

    for (short int i = 0; i < BiT_Permutations.size(); ++i) {
      Lex.insert(BiT_Permutations[i]);
      BiT_Permutations[i].clear();
    }


    for (itr = Lex.begin(); itr != Lex.end(); itr++) {
      (*itr) == original_key ? found_at = search_iterations : search_iterations++;

      Solution.push_back((*itr)[(*itr).length() - 1]);
    }

    this->Lex.clear();
    return Solution.append(to_string(found_at));
  }

public:
  string get_Encrypted() {
    return this->Encrypted;
  }
  

};


deque<string> Convert_Binary(const string& message) {

  deque<string> MSG; 
  vector<int>Binary_iMSG; 
  string holder;
  
  int count_bits = 0;
  int current_symbol_ascii = 0;
  int Binary_num[32]; int bit;


  for (unsigned short int i = 0; i < message.size(); ++i) {
    current_symbol_ascii = (int)message[i];
    Binary_iMSG.push_back(current_symbol_ascii);
  }

  for (unsigned short int i = 0; i < Binary_iMSG.size(); i++) {

    current_symbol_ascii = Binary_iMSG[i];


    while (current_symbol_ascii != 0) {
      Binary_num[count_bits] = current_symbol_ascii % 2;
      current_symbol_ascii = current_symbol_ascii / 2;
      count_bits++;
    }

    for (short int j = count_bits - 1; j >= 0; --j) {
      bit = Binary_num[j];
      holder.append(to_string(bit));

    }
    if (holder.size() < 8) {
      int empty_bits = 8 - holder.length() - 1;
      for (int i = 0; i <= empty_bits; ++i) {
        holder.insert(0, "0");
      }
    }
    MSG.push_back(holder);
    holder.clear(); count_bits = 0;
  }

  return MSG;
}


std::string encrypt_layer2(string message){
    string encrypted;
    deque<string>Binary_Message = Convert_Binary(message);
  
    Encrypted_unit* Encrypted_message = new Encrypted_unit(Binary_Message);
  
    encrypted = Encrypted_message->get_Encrypted();
    delete Encrypted_message;
    return encrypted;    
}



string decrypt_layer2(string& Robot_reply){
   string decrypted;
  
   Decrypted_unit* Decrypted_message = new Decrypted_unit(Robot_reply);
   decrypted = Decrypted_message->get_decrypted();
  
   delete Decrypted_message;
  
   return decrypted;
}



string encryption_layer1(string s)
{
    int l = s.length();
    int b = ceil(sqrt(l));
    int a = floor(sqrt(l));
    string encrypted;
    if (b * a < l) {
        if (min(b, a) == b) {
            b = b + 1;
        }
        else {
            a = a + 1;
        }
    }

    char arr[a][b];
    memset(arr, ' ', sizeof(arr));
    int k = 0;
     
    for (int j = 0; j < a; j++) {
        for (int i = 0; i < b; i++) {
            if (k < l){
                arr[j][i] = s[k];
            }
            k++;
        }
    }
 

    for (int j = 0; j < b; j++) {
        for (int i = 0; i < a; i++) {
            encrypted = encrypted +
                         arr[i][j];
        }
    }
    return encrypted;
}


string decryption_layer1(string s){
    int l = s.length();
    int b = ceil(sqrt(l));
    int a = floor(sqrt(l));
    string decrypted;
  

    char arr[a][b];
    memset(arr, ' ', sizeof(arr));
    int k = 0;
     

    for (int j = 0; j < b; j++) {
        for (int i = 0; i < a; i++) {
            if (k < l){
                arr[j][i] = s[k];
            }
            k++;
        }
    }
 

    for (int j = 0; j < a; j++) {
        for (int i = 0; i < b; i++) {
            decrypted = decrypted +
                         arr[i][j];
        }
    }
    return decrypted;
}