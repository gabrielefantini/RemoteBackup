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
int setupUsers(std::map<std::pair<std::string,std::string>,int> &m){
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
void printUserMap(std::map<std::pair<std::string,std::string>,int> &m){
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
int addUser(std::map<std::pair<std::string,std::string>,int> &m,std::string &usr,std::string &path){
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
std::pair<std::string,int> getBackupDir(std::map<std::pair<std::string,std::string>,int> &user_map,std::string &usr,std::string &path){
    auto search=user_map.find(std::make_pair(usr,path));
    int folder_num;
    if(search==user_map.end()){
        //coppia user-folder non esiste -> la creo
        folder_num=addUser(user_map,usr,path);
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

std::string getTmpDir(std::string &bpath){
    //std::cout<<"backup: "<<bpath<<std::endl;

    //setup: create tmp folder (if not exists yet)
    std::string parent_path=fs::path(bpath).parent_path().string();
    std::string tmp=std::string(fs::path(parent_path).parent_path().string()+"/tmp");
    fs::create_directory(tmp);

    return tmp;
}

std::set<std::string> checkForFile(std::map<std::string,std::string> &client_map,std::map<std::string,std::string> &server_map){
    int cnt=0;
    std::set<std::string> fset;
    for(auto& x: client_map){
        std::string path=x.first;
        if(x.second=="")
            std::cout << path << " è una directory\n";
        else {
            //caso file
            std::cout << path << " è un file\n";
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
                    std::cout << "file " << path << " MANCANTE.\n";
                    fset.insert(path);
                }
                cnt++;
            }else{
                if(it->second!=x.second){
                    //CASO DI FILE MODIFICATO: stesso path, diverso digest
                    std::cout << "file " << path << " MODIFICATO.\n";
                    fset.insert(path);
                    cnt++;
                }
            }
        }
    }
    if(cnt==0)
        std::cout<<"nessuna modifica richiesta.\n";
    return fset;
}

std::string getServerPath(std::string clientPath, std::string clientDir,std::string backupDir){
    std::cout<<"\nGET SERVER PATH {entro}\n";
    std::cout<<"client path: "<<clientPath<<"\n";
    std::cout<<"client dir: "<<clientDir<<"\n";
    std::cout<<"backup dir: "<<backupDir<<"\n";
    std::string serverPath;
    int pos=clientDir.length();
    serverPath=std::string(backupDir+clientPath.substr(pos));
    std::cout<<"==> "<<serverPath<<"\n";
    std::cout<<"GET SERVER PATH {esco}\n\n";
    return serverPath;
}

void updateBackupFolder(std::map<std::string,std::string> &client_map,std::map<std::string,std::string> &server_map,std::string &tmp,std::string &backup,std::string client_dir){
    std::cout<<"UPDATE BACKUP FOLDER\n";
    for(auto& x: client_map){
            std::string path=x.first;
            if(x.second=="") {
                //caso directory
                if(server_map.find(path)==server_map.end()){
                    //creazione dir
                    std::string newpath=getServerPath(path,client_dir,backup);
                    std::cout<<"DIRECTORY MANCANTE!\n";
                    std::cout<<"dir "<<newpath<<" mancante -> creazione directory.\n";
                    fs::create_directory(newpath);
                }
            }
            else {
                //caso file
                std::map<std::string,std::string>::iterator it=server_map.find(path);
                if(it==server_map.end()){
                    bool fnd=false;
                    for(auto& y: server_map)
                        if(y.second==x.second && fs::path(y.first).filename().string()==fs::path(path).filename().string() && fs::path(y.first).parent_path().string()!=fs::path(path).parent_path().string()){
                            //CASO FILE SPOSTATO: path diverso, stesso digest, stesso filename
                            //spostamento file
                            std::string oldpath=getServerPath(y.first,client_dir,backup);
                            std::string newpath=getServerPath(path,client_dir,backup);
                            std::cout<<"FILE SPOSTATO!\n";
                            std::cout<<oldpath<<" -> "<<newpath<<std::endl;
                            fs::copy_file(oldpath,newpath);
                            fs::remove(oldpath);
                            fnd=true;
                        }else if(y.second==x.second && fs::path(y.first).filename().string()!=fs::path(path).filename().string() && fs::path(y.first).parent_path().string()==fs::path(path).parent_path().string()){
                            //CASO FILE RINOMINATO: path diverso(ma stesso parent_path), stesso digest, filename diverso
                            //aggiornamento file
                            std::string oldpath=getServerPath(y.first,client_dir,backup);
                            std::string newpath=getServerPath(path,client_dir,backup);
                            std::cout<<"FILE RINOMINATO!\n";
                            std::cout<<oldpath<<" -> "<<newpath<<std::endl;
                            fs::rename(oldpath,newpath);
                            fnd=true;
                        }
                    if(!fnd){
                        //CASO FILE MANCANTE: path diverso(anche parent_path)
                        //inserimento file
                        std::string newfile=std::string(tmp+"/"+x.second);
                        std::string newpath=getServerPath(path,client_dir,backup);
                        std::cout<<"FILE MANCANTE!\n";
                        std::cout<<newfile<<" -> "<<newpath<<std::endl;
                        fs::copy_file(newfile,newpath);
                    }
                }else{
                    if(it->second!=x.second){
                        //CASO DI FILE MODIFICATO: stesso path, diverso digest
                        //aggiornamento file
                        std::string newfile=std::string(tmp+"/"+x.second);
                        std::string filepath=getServerPath(path,client_dir,backup);
                        std::cout<<"FILE MODIFICATO!\n";
                        std::cout<<newfile<<" -> "<<filepath<<std::endl;
                        fs::remove(filepath);
                        fs::copy_file(newfile,filepath);
                    }
                }

            }
        }
        for(auto& x: server_map){
            std::string path=x.first;
            if(client_map.find(path)==client_map.end()){
                std::string oldpath=getServerPath(x.first,client_dir,backup);
                std::cout<<"FILE/DIRECTORY SUPERFLUI!\n";
                if(x.second=="")
                    std::cout<<"dir "<<oldpath<<" da eliminare.\n";
                else
                    std::cout<<"file "<<oldpath<<" da eliminare.\n";
                //rimozione file
                fs::remove(oldpath);
            }

        }
}

std::string getMapPath(std::string &backupPath){
    std::cout<<"GET MAP PATH\n";
    std::string parent_path=fs::path(backupPath).parent_path().string();
    return std::string(parent_path+"/localMap.txt");
}

int setupLocalMap(std::string &mapPath,std::map<std::string,std::string> &m){
    if(fs::exists(mapPath) && fs::is_regular(mapPath)){
        FILE *fp;
        char path[MAXLEN+1],hash[MAXLEN+1];
        fp=fopen(mapPath.c_str(),"r+");
        if(fp==nullptr){
            std::cout<<"error opening file "<<mapPath<<"\n";
            return -1;
        }
        while(fscanf(fp,"%s %s",path,hash)!=EOF)
            if(hash!="directory")
                m.insert(std::pair<std::string,std::string>(path,hash));
            else
                m.insert(std::pair<std::string,std::string>(path,""));

    fclose(fp);
        return 1;   //file trovato, map caricata
    }
    return 0;   //file non esistente, map rimane vuota
}

int saveLocalMap(std::string &mapPath,std::map<std::string,std::string> &m){
    std::cout<<"SAVE LOCAL MAP\n";
    //rimozione vecchio file
    if(fs::exists(mapPath) && fs::is_regular(mapPath))
        fs::remove(mapPath);
    //creazione nuovo file
    FILE *fp;
    fp=fopen(mapPath.c_str(),"w+");
    if(fp==nullptr){
        std::cout<<"error opening file "<<mapPath<<"\n";
        return -1;
    }
    for (auto& x: m)
        if(x.second!="")
            fprintf(fp,"%s %s\n",x.first.c_str(),x.second.c_str());
        else
            fprintf(fp,"%s directory\n",x.first.c_str());

    fclose(fp);
    return 0;
}

void flushTmp(std::string &tmp){
    fs::remove_all(tmp);
    std::cout<<tmp<<" folder cleared.\n";
}