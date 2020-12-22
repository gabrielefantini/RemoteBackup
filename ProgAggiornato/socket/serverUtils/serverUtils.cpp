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
    int index=0,value;
    std::pair<std::string,std::string> key;
    for (auto& x: m) {
        key=x.first;
        value=x.second;
        if(usr.compare(key.first)==0)
            if(index==value)
                index++;
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
std::string get_backup_dir(std::map<std::pair<std::string,std::string>,int> &user_map,std::string &usr,std::string &path){
    auto search=user_map.find(std::make_pair(usr,path));
    int folder_num;
    if(search==user_map.end()){
        //coppia user-folder non esiste -> la creo
        folder_num=add_user(user_map,usr,path);
        if(folder_num==-1){
            std::cout<<"error during users update.\n";
            return std::string("");
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

    backup_folder<<BACKUP<<usr<<"/"<<folder_num<<"/"<<filename;
    std::cout << "backup folder: "<<backup_folder.str()<< std::endl;
    return backup_folder.str();
}

