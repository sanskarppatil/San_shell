#include<iostream> 
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<cstdlib>
#include<string>
#include<vector>
#include<algorithm>
#include<fcntl.h>
#include<signal.h>
#include<sstream>
#include<fstream>
#include<cstring> 
#include<cerrno>
#include<limits.h>
#include<stdio.h>
#include<sys/stat.h>
int main(){
  while(1){
        char currentpath[PATH_MAX];
	if(getcwd(currentpath,sizeof(currentpath))!=NULL){
		std::cout<<currentpath<<" :$ ";
	}
	else{
		std::cout<<"$ ";
	}
        std::string input;
        getline(std::cin,input);
	std::istringstream instr(input);
        std::string word;
	std::vector<std::string> tokens;
	while(instr >> word) tokens.push_back(word);
        if(tokens.empty()){
		std::cout<<std::endl;
		continue;          
        }
	if(*tokens.begin()=="exit") break;
        if(tokens[0] == "cd") {
        if(tokens.size() < 2) {
            std::cerr << "cd: missing argument" << std::endl;
        }
	else {
            // Correct parenthesis placement!
            if(chdir(tokens[1].c_str()) != 0) {
                perror("cd"); // Much better error reporting than a hardcoded string
            }
        }
        	continue;
   	 }
	else if(tokens[0]=="pwd"){
		char currentpath[PATH_MAX];
		if(getcwd(currentpath,sizeof(currentpath))!=NULL){
			std::cout<<currentpath<<std::endl;
		}
		else std::cerr<<"pwd not working"<<std::endl;
		continue;
	}
	else if(tokens[0]=="which"){
		char *path = getenv("PATH");
		if(tokens.size()<=1) continue;
		std::string path_string(path);
		std::stringstream ss(path_string);
	        std::string paths;
		while(getline(ss,paths,':')){
			std::string fullpath = paths;
			fullpath += "/";
			fullpath += tokens[1];
			const char *c_path = fullpath.c_str();
			struct stat sb;
			if(stat(c_path,&sb) == 0){
				std::cout<<fullpath<<std::endl;
			}						
		}
		continue;
	}
	std::vector<char*> args;
	for(auto &s:tokens){
		args.push_back(const_cast<char*>(s.c_str()));

	}

        pid_t pid = fork();
	if(pid==0){
		execvp(args[0],args.data());
		if(errno == ENOENT) std::cerr << args[0] << ": command not found" << std::endl;
		else{
			perror("exec failed");
		} 
		exit(1);
	}
	else if(pid>0){
		wait(NULL);	
	}
        else{
		perror("fork failed");
	}
  }
  return 0;
}


