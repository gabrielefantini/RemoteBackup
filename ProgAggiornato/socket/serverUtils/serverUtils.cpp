//
// Created by lollo on 22/12/20.
//

#include "serverUtils.h"

/*
 * Inserisce nella map passata come parametro l'elenco di "client path index" presenti
 * nel file di configurazione.
 *
 * param {m} : map <<string,string>,int>
 *
 * return : 0 (ok)
 *         -1 (error)
 * */
int setup_users(std::map<std::pair<std::string,std::string>,int> &m){
    FILE *fp;
    char usr[MAXLEN+1],path[MAXLEN+1];
    int num;
    fp=fopen(USERS_FILE,"r+");
    if(fp==nullptr){
        std::cout<<"error opening file "<<USERS_FILE<<"\n";
        return -1;
    }
    while(fscanf(fp,"%s %s %d",usr,path,&num)!=EOF){
        m.insert(std::pair<std::pair<std::string,std::string>,int>(std::make_pair(usr,path),num));
    }
    fclose(fp);
    return 0;
}

/*
 * Stampa la map di users.
 *
 * param {m} : map <<string,string>,int>
 * */
void print_user_map(std::map<std::pair<std::string,std::string>,int> &m){
    std::cout<<"users file: "<<std::endl;
    if(m.size()==0){
        std::cout<<"[empty]"<<std::endl;
        return;
    }
    for (auto& x: m) {
        std::pair<std::string,std::string> key=x.first;
        int value=x.second;
        std::cout<<key.first+","+key.second+" : "<<value<<std::endl;
    }
    std::cout<<"\n";
}

/*
 * In caso in cui la directory dell'utente non sia ancora presente, viene creata ed aggiunta
 * alle strutture dati di competenza
 *
 * param {m}    : map <<string,string>,int>
 * param {usr}  : string
 * param {path} : string
 *
 * return : int (ok)
 *          -1  (error)
 * */
int add_user(std::map<std::pair<std::string,std::string>,int> &m,std::string &usr,std::string &path){
    int index,max=0,value,i;
    std::pair<std::string,std::string> key;
    std::set<int> indexes;
    for (auto& x: m) {
        key=x.first;
        value=x.second;
        if(usr.compare(key.first)==0) {
            indexes.insert(value);
            if(value>max)
                max=value;
        }
    }
    for(i=0;i<=max+1;i++)
        if(indexes.find(i)==indexes.end()){
            index=i;
            break;
    }
    m.insert(std::pair<std::pair<std::string,std::string>,int>(std::make_pair(usr,path),index));
    //aggiorno anche il file
    FILE *fp;
    fp=fopen(USERS_FILE,"a+");
    if(fp==nullptr){
        std::cout<<"error opening file "<<USERS_FILE<<"\n";
        return -1;
    }
    fprintf(fp,"%s %s %d\n",usr.c_str(),path.c_str(),index);
    fclose(fp);
    return index;
}

/*
 * Ritorna il path di backup relativo alla directory indicata dal client.
 *
 * param {m}    : map <<string,string>,int>
 * param {usr}  : string
 * param {path} : string
 *
 * return : string (ok)
 *          ""     (error)
 * */
std::pair<std::string,int> get_backup_dir(std::map<std::pair<std::string,std::string>,int> &user_map,std::string &usr,std::string &path){
    auto search=user_map.find(std::make_pair(usr,path));
    int folder_num;
    if(search==user_map.end()){
        //coppia user-folder non esiste -> la creo
        folder_num=add_user(user_map,usr,path);
        if(folder_num==-1){
            std::cout<<"error during users update.\n";
            return std::make_pair(std::string(""),-1);
        }
        std::cout<<"added user-folder, index: "<<folder_num<<std::endl;
    }else{
        folder_num=search->second;
        std::cout<<"found user-folder, index: "<<folder_num<<std::endl;
    }
    //the backup folder should be: backup/{client}/{value}/{filename}
    std::string filename=fs::path(path).filename().string();
    std::stringstream backup_folder;

    std::cout << "dir watched: "+filename << std::endl;
    fs::path backupPath=fs::canonical(fs::path(BACKUP));
    backup_folder<<backupPath.string()<<"/"<<usr<<"/"<<folder_num<<"/"<<filename;
    return std::make_pair(backup_folder.str(),folder_num);
}

std::string get_tmp_dir(std::string &bpath){
    std::cout<<"backup: "<<bpath<<std::endl;

    //setup: create tmp folder (if not exists yet)
    std::string parent_path=fs::path(bpath).parent_path().string();
    std::string tmp=std::string(fs::path(parent_path).parent_path().string()+"/tmp");
    fs::create_directory(tmp);

    return tmp;
}

std::set<std::string> check_for_file(std::map<std::string,std::string> &client_map,std::map<std::string,std::string> &server_map){
    int cnt=0;
    std::set<std::string> fset;
    for(auto& x: client_map){
        std::string path=x.first;
        if(x.second=="")
            std::cout << x.first << " is a dir\n";
        else {
            //caso file
            //std::cout << x.first << " is a file\n";
            std::map<std::string,std::string>::iterator it=server_map.find(path);
            if(it==server_map.end()){
                bool fnd=false;
                for(auto& y: server_map)
                    if(y.second==x.second && fs::path(y.first).filename().string()==fs::path(path).filename().string() && fs::path(y.first).parent_path().string()!=fs::path(path).parent_path().string()){
                        //CASO FILE SPOSTATO: path diverso, stesso digest, stesso filename
                        fnd=true;
                    }else if(y.second==x.second && fs::path(y.first).filename().string()!=fs::path(path).filename().string() && fs::path(y.first).parent_path().string()==fs::path(path).parent_path().string()){
                        //CASO FILE RINOMINATO: path diverso(ma stesso parent_path), stesso digest, filename diverso
                        fnd=true;
                    }
                if(!fnd){
                    //CASO FILE MANCANTE: path diverso(anche parent_path)
                    std::cout << "file " << path << " missing -> calling client.\n";
                    fset.insert(path);
                }
                cnt++;
            }else{
                if(it->second!=x.second){
                    //CASO DI FILE MODIFICATO: stesso path, diverso digest
                    std::cout << "file " << path << " changed -> calling client.\n";
                    //TO_DO call file
                    fset.insert(path);
                    cnt++;
                }
            }
        }
    }
    if(cnt==0)
        std::cout<<"no mod required.\n";
    //TO_DO call ok
    return fset;
}