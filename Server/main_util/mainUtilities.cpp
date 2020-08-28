//
// Created by neppa on 24/08/20.
//

#include "mainUtilities.h"

//get_server_dir: dato il path della cartella da monitorare lato client, viene composto il path della cartella destinazione lato server
std::string get_server_dir(const std::string &username,const std::string &path){
    std::string server_path=BACKUP_FOLDER+std::string("/")+username+std::string("/")+fs::path(path).filename().string();
    return server_path;
}
//setup_dir: cerca se la directory relativa al path di un certo utente esiste già (return 1), altrimenti la crea (return 0)
int setup_dir(const std::string &path){
    if(fs::exists(path) && fs::is_directory(path)) {
        std::cout << path << " found in backups.\n\n";
        return 1;
    }else{
        std::cout<<path<<" missing!\n";
        fs::create_directory(path);
        std::cout<<path<<" created correctly in backups.\n\n";
        return 0;
    }
}
//cs_to_sc (client/server to server/client): traduce un path della cartella client/server nel corrispettivo server/client (con gli opportuni riferimenti "from" e "to"
void cs_to_sc(std::string &path,const std::string &from,const std::string &to){
    // ./../../backups/client1/dir1
    size_t pos=path.find(from);
    std::string substr=path.substr(pos+from.length());
    std::string np=to+substr;
    path.replace(path.begin(),path.end(),np);
    return;
}
//setup_users: legge da file "users.txt" (possibili ridefinizioni) le corrispondenze usr -> cartella client e le inserice in una map
/*int setup_users(std::map<std::string,std::string> &m){
    FILE *fp;
    char usr[MAXLEN+1],path[MAXLEN+1];

    fp=fopen(USERS_FILE,"r+");
    if(fp==nullptr){
        std::cout<<"error opening file "<<USERS_FILE<<"\n";
        return -1;
    }
    while(fscanf(fp,"%s %s",usr,path)!=EOF){
        m.insert(std::pair<std::string,std::string>(usr,path));
    }
    fclose(fp);
    return 0;
}*/
//setup_map: dato il path lato server crea e restituisce una map con le sue coppie <path,digest>
std::map<std::string,std::string> setup_map(std::string &folder_path){
    std::map<std::string,std::string> map;
    for (auto &file : fs::recursive_directory_iterator(folder_path)) {
        std::string path=file.path().string();
        int p = add(path, map);
        //manca gestione errori
    }
    return map;
}
//print_map
void print_map(std::map<std::string,std::string> &m){
    for (auto& x: m) {
        std::string second=x.second.length()>0 ? x.second : "directory";
        std::cout << x.first << " : " << second << std::endl;
    }
    std::cout<<"\n";
}
//compare_and_process: confronta i digest client e server e aggiorna il server di conseguenza (versione "locale" di testing, le azioni eseguite cambieranno quando ci sarà comunicazione con il client)
void compare_and_process(std::map<std::string,std::string> &client_map,std::map<std::string,std::string> &server_map,const std::string &cpath, const std::string &spath){
    int cnt=0;
    for(auto& x: client_map){
        std::string path=x.first;
        cs_to_sc(path,cpath,spath);
        if(x.second=="") {
            //caso directory
            //std::cout << x.first << " is a dir\n";
            if(server_map.find(path)==server_map.end()){
                std::cout<<"dir "<<path<<" missing -> create_directory.\n";
                fs::create_directory(path);
                add(path,server_map);
                cnt++;
            }

        }
        else {
            //caso file
            //std::cout << x.first << " is a file\n";
            std::map<std::string,std::string>::iterator it=server_map.find(path);
            if(it==server_map.end()){
                bool fnd=false;
                for(auto& y: server_map)
                    if(y.second==x.second && fs::path(y.first).filename().string()==fs::path(path).filename().string() && fs::path(y.first).parent_path().string()!=fs::path(path).parent_path().string()){
                        //CASO FILE SPOSTATO: path diverso, stesso digest, stesso filename
                        std::cout << "file has to be moved: " << y.first << " -> " << path << "\n";
                        fs::copy_file(y.first,path);
                        fs::remove(y.first);
                        server_map.insert(std::pair<std::string,std::string>(path,y.second));
                        remove(y.first,server_map);
                        fnd=true;
                    }else if(y.second==x.second && fs::path(y.first).filename().string()!=fs::path(path).filename().string() && fs::path(y.first).parent_path().string()==fs::path(path).parent_path().string()){
                        //CASO FILE RINOMINATO: path diverso(ma stesso parent_path), stesso digest, filename diverso
                        std::cout << "file renamed: " << y.first << " -> " << path << "\n";
                        fs::rename(y.first,path);
                        server_map.insert(std::pair<std::string,std::string>(path,y.second));
                        remove(y.first,server_map);
                        fnd=true;
                    }
                if(!fnd){
                    //CASO FILE MANCANTE: path diverso(anche parent_path)
                    std::cout << "file " << path << " missing -> copying to server.\n";
                    fs::copy_file(x.first,path);
                    server_map.insert(std::pair<std::string,std::string>(path,x.second));
                }
                cnt++;
            }else{
                if(it->second!=x.second){
                    //CASO DI FILE MODIFICATO: stesso path, diverso digest
                    std::cout << "file " << path << " changed -> copying to server.\n";
                    fs::remove(path);
                    fs::copy_file(x.first,path);
                    modify(path,server_map);
                    cnt++;
                }
            }

        }
    }
    for(auto& x: server_map){
        std::string path=x.first;
        cs_to_sc(path,spath,cpath);
        /*if(x.second=="directory") {
            //caso directory
            //std::cout << x.first << " is a dir\n";
            if(client_map.find(path)==client_map.end()){
                std::cout<<"dir "<<x.first<<" has to be deleted.\n";
                fs::remove(x.first);
                remove(x.first,server_map);
                cnt++;
            }
        }
        else {
            //caso file
            //std::cout << x.first << " is a file\n";
            if(client_map.find(path)==client_map.end()){
                std::cout<<"file "<<x.first<<" has to be deleted.\n";
                fs::remove(x.first);
                remove(x.first,server_map);
                cnt++;
            }

        }*/
        if(client_map.find(path)==client_map.end()){
            if(x.second=="")
                std::cout<<"dir "<<x.first<<" has to be deleted.\n";
            else
                std::cout<<"file "<<x.first<<" has to be deleted.\n";
            fs::remove(x.first);
            remove(x.first,server_map);
            cnt++;
        }

    }
    if(cnt==0)
        std::cout<<"no mod required.\n";

}
