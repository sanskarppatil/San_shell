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

int main(){
  while(1){
        char currentpath[PATH_MAX];
	if(getcwd(currentpath,sizeof(currentpath))!=NULL){
		std::cout<<currentpath<<" $";
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
	std::vector<char*> args;
	for(auto &s:tokens){
		args.push_back(const_cast<char*>(s.c_str()));

	}
	args.push_back(nullptr);
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
