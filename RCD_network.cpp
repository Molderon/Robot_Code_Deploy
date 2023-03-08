#include "RCD_server.h"

unique_ptr<string> IP_v4;
static bool network_tested = false;
static char* inner_net;


bool net_test(){
    if(network_tested == true){return true;}
    char hostbuffer[256];
    struct hostent *host_entry;
    unsigned short int host_name = 0;
    int pfile_keys[2];
    
    if(pipe(pfile_keys) == -1){
      //wrire to LED display and error log
      cerr<<"PiPe file for transer of IP was not initialized\n";
      return false;
    }
    else{
      ifstream File; string IP; 
      char space_key = ' ';
      short unsigned int index = 0;
      
      system("hostname -I > IP_buffer.txt");
      /*
        Here I didn't figure out how to
        get the out-put of an System() call to a pfile :/
        I will rewrite it sometime soon.
      */

      File.open("IP_buffer.txt", ios::in);
      if(File.is_open())
      {
        File.close();
        File.open("IP_buffer.txt", ios::in);

        if(File.is_open()){
          getline(File, IP);
          index = IP.find(space_key);

          if(index != string::npos)
          {
            IP.erase(index, IP.size());
            File.close(); 
            system("rm IP_buffer.txt");
            
            int ip_size = IP.size();
            write(pfile_keys[1], &ip_size, sizeof(int));
            

            if(write(pfile_keys[1], IP.c_str(), strlen(IP.c_str()))<0){
              return false;
            }
            close(pfile_keys[1]);
          }

          else
          {
           cerr<<"command failed";
           return false; system("rm IP_buffer.txt");
           File.close();
          }
        }
      }
      else{
        cerr<<"[System] -> local IP address could not be resolved\n";
        return false;
      }
    }
    
    close(pfile_keys[1]);

    int ip_size = 0;
    read(pfile_keys[0], &ip_size, sizeof(int));

    char raw_IP[ip_size];

    // Generate Device information    
    read(pfile_keys[0], &raw_IP[0], (sizeof(char)*(ip_size)));
    host_name = gethostname(hostbuffer, sizeof(hostbuffer));
    host_entry = gethostbyname(hostbuffer);
    inner_net = inet_ntoa(*(struct in_addr*)host_entry->h_addr_list[0]);
    close(pfile_keys[0]);

    if(host_entry == NULL || host_name == -1 || raw_IP == NULL ){
        cerr<<"[System]::Networking Failure\n";
        return false;
    }
    
    IP_v4 = make_unique<string>();
    (*IP_v4).assign(raw_IP); // I dislike C-styple strings. >:(
    (*IP_v4).pop_back();
    //(*IP_v4).assign("127.0.0.1"); //this is for testing purpose

    cout<<"[System]::Device Name: "<<hostbuffer<<endl;
    cout<<"[System]::Inner Net: "<<inner_net<<endl;
    cout<<"[System]::IPv4: "<<(*IP_v4)<<"\n\n\n";
    

    return true;
}



const char* create_addr(string* address){
  
  (*address).append("tcp://" + (*IP_v4) +":"+to_string(default_port));
  cout<<"address to connect: "<<(*address)<<endl;
  return (*address).c_str();
}



pair<bool,zsock_t*> Authenticate_Master(){
    bool allow_connection = false;
    string* address = new string;
    zsock_t *responder = zsock_new(ZMQ_REP);
    
    unsigned short int port_bind = zsock_bind(responder, create_addr(address));
    delete address; IP_v4.~unique_ptr();

    if(port_bind != default_port){
        cerr<<"Socket for port: "<<port_bind<<", could not be binded.\n";
        zsock_destroy(&responder);
        return make_pair(allow_connection, nullptr);
    }
    
    string* Pass_phrase = new string(zstr_recv(responder));
    (*Pass_phrase).assign(decryption_layer1((*Pass_phrase).c_str()));

     

    if(sizeof(Pass_phrase) == 0){
        zsock_destroy(&responder);
        cerr<<"\n[System]:: Faulty connection handeling\n";
        delete Pass_phrase;
        return make_pair(allow_connection, nullptr);
    }
        else if(!strcmp(Pass_phrase->c_str() , "I'm your master-recreator...")){
            cerr<<"\n[System]:: Authentication Violated!\n ...Terminating\n";
            zsock_destroy(&responder);
            delete Pass_phrase;
            return make_pair(allow_connection, nullptr);
        }
            else{
                delete Pass_phrase;
                string approved = "Connection Approved";
                approved = encryption_layer1(approved); 
                allow_connection = true;     
                zstr_send(responder, approved.c_str());
                cout<<"\n[System]::Thy master has arrived \\m/\n\n";
                return make_pair(allow_connection, responder);
            }
}





// Here lives Encryption :p

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



string decrypt_layer2(string Robot_reply){
   string decrypted(Robot_reply);  
   Decrypted_unit* Decrypted_message = new Decrypted_unit(decrypted);
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
     
    // Fill the matrix row-wise
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
    string decrypted(s);
    int l = decrypted.length();
    int b = ceil(sqrt(l));
    int a = floor(sqrt(l));

    char arr[a][b];
    memset(arr, ' ', sizeof(arr));
    int k = 0;


    for (int j = 0; j < b; j++) {
        for (int i = 0; i < a; i++) {
            if (k < l){
                arr[j][i] = decrypted[k];
            }
            k++;
        }
    }
 
    decrypted.clear();

    for (int j = 0; j < a; j++) {
        for (int i = 0; i < b; i++) {
            decrypted = decrypted +
                         arr[i][j];
        }
     }
    return decrypted;
}