#!/usr/bin/env python

import os,sys,subprocess, time

####################CONFIG PATH##############################
download_path  = '/home/bottos/testAbi/'        #############
contracts_path = '/home/bottos/contracts/'      #############
#############################################################

eos_script = download_path + "eosio_build.sh"

#cmd tables
git_cmd = "git clone https://github.com/EOSIO/eos.git " + download_path
update_cmd = "cd " + download_path + "; git submodule update --init --recursive"
set_permission = "chmod +x " + eos_script
run_eos_build = "cd " + download_path + "; ./eosio_build.sh"
git_cmd_bottos = "rm -rf contracts; git clone https://github.com/bottos-project/contracts.git"
copy_cmd_bottos = "cp " + contracts_path+"* /home/bottos/testAbi/tools"


def clone_eos_code():
 	global git_cmd, update_cmd, set_permission, run_eos_build, git_cmd_bottos, copy_cmd_bottos
	
	if len(sys.argv)>1 and sys.argv[1] == 'deploy':
		pass
	else:
		git_cmd, update_cmd, set_permission, run_eos_build, git_cmd_bottos = ' '*5
	
	cmd_list = [ git_cmd, update_cmd, set_permission, run_eos_build, git_cmd_bottos, copy_cmd_bottos]
	
	print "==>",git_cmd, update_cmd, set_permission, run_eos_build, git_cmd_bottos, "<=="	
	for cmd in cmd_list:
		if not cmd:
			continue
        	P =subprocess.Popen(cmd, shell=True)
        	P.communicate()


def loop_dirs():
	
	abi_begin = False
	for (root, dirs, files) in os.walk(download_path+"tools"):
		for filename in files:
			#print os.path.join(root,filename)
			if ".wasm" in os.path.join(root,filename)[-5:]:
				cpp_file = os.path.join(root,filename)[:-4] + 'cpp'
				hpp_file = os.path.join(root,filename)[:-4] + 'hpp'
				abi_file = os.path.join(root,filename)[:-4] + 'abi'
				#print "now open: ", hpp_file
				
				with open(cpp_file, 'r') as cppf:
					for line in cppf.readlines():
						if 'ABI INFO BEGIN' in line:
							if os.path.exists(hpp_file):
								os.remove(hpp_file)	
							abi_begin = True
							with open(hpp_file, 'a') as hppf:
								header_info = '#include ' + download_path + 'contracts/eosiolib/types.h'
								hppf.write(header_info)
							continue
						
						if abi_begin:
							if 'ABI INFO END' in line:
								break
									
							with open(hpp_file, 'a') as hppf:
								hppf.write(line)
								
									
				gen_bto_abi(abi_file, hpp_file)
				#print os.path.join(root,hpp_file)	
		for dirc in dirs:
			pass
			#print os.path.join(root,dirc)


def gen_bto_abi(abi_path, hpp_path):
	genabi_cmd =  download_path + 'tools/eosiocpp.in -g ' + abi_path + ' ' + hpp_path + '; sed -i "" "s/bytes/string/g"' + ' ' + abi_path + '; cat ' + abi_path

	print '\n\ngenabi_cmd:', genabi_cmd, '\n\n'
        P =subprocess.Popen(genabi_cmd, shell=True)
        P.communicate()

if __name__ == '__main__':
    #print 'here'
    
    	clone_eos_code()
    	#loop_dirs()
    	#print 'done'

