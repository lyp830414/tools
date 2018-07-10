#!/usr/bin/env python

import os,sys,subprocess, time

####################CONFIG PATH##############################
download_path  = '/home/bottos/EOS/'            #############
contracts_path = '/home/bottos/contracts/'      #############
external_abi_path = '/home/bottos/contracts/lib' ############
#############################################################

eos_script = download_path + "eosio_build.sh"
eosio_abigen_path = download_path + 'build/programs/eosio-abigen/eosio-abigen'
eosio_bin_path    = download_path + 'bin'

#cmd tables
git_cmd = 'git clone https://github.com/EOSIO/eos.git ' + download_path
update_cmd = 'cd ' + download_path + '; git submodule update --init --recursive'
set_permission = 'chmod +x ' + eos_script
run_eos_build = 'cd ' + download_path + '; ./eosio_build.sh'
git_cmd_bottos = 'rm -rf contracts; git clone https://github.com/bottos-project/contracts.git; sudo apt-get install -y gcc-4.8-multilib g++-4.8-multilib'
copy_cmd_bottos = 'mkdir ' + eosio_bin_path + ' 2>/dev/null; cp ' + eosio_abigen_path + ' ' +  eosio_bin_path + '; cp -rf ' + contracts_path +'* ' + download_path + 'tools'


def clone_eos_code():
 	global git_cmd, update_cmd, set_permission, run_eos_build, git_cmd_bottos, copy_cmd_bottos
	
	if len(sys.argv)>1 and sys.argv[1] == 'deploy':
    		if os.path.exists(download_path):
			os.system('rm -rf '+download_path)
			#os.system('ls -l ' + download_path)
		pass
	else:
		git_cmd, update_cmd, set_permission, run_eos_build, git_cmd_bottos = ' '*5
	
	cmd_list = [ git_cmd, update_cmd, set_permission, run_eos_build, git_cmd_bottos, copy_cmd_bottos]
	
	print "==>",git_cmd, update_cmd, set_permission, run_eos_build, git_cmd_bottos, "<=="	
	for cmd in cmd_list:
		if not cmd:
			continue
        	print 'NOW CMD:', cmd
		P =subprocess.Popen(cmd, shell=True)
        	P.communicate()


def find_external_abi_struct(struct_name):
	line_begin = False
	line_end   = False
	struct_lines = []
	for (root, dirs, files) in os.walk(external_abi_path):
		for filename in files:	
			if not ('.hpp' in filename) or not ('.h' in filename):
				continue
			filepath = root+'/'+filename
			with open(filepath) as external_hppf:
				for line in external_hppf.readlines():
					if not line_begin:
						if not struct_name in line:
							continue
						if not 'struct ' in line:
							continue
					
						line_begin = True
						struct_lines.append(line)
					else:
						struct_lines.append(line)
						
						if '}' in line:
							line_end = True
							return struct_lines
					
					

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
				
				if not os.path.exists(cpp_file):
					continue
				
				with open(cpp_file, 'r') as cppf:
					for line in cppf.readlines():
						if 'ABI INFO BEGIN' in line:
							if os.path.exists(hpp_file):
								os.remove(hpp_file)	
							abi_begin = True
							with open(hpp_file, 'a') as hppf:
								header_info = '#include ' + '\"' + download_path + 'contracts/eosiolib/types.h' + '\"'
								hppf.write(header_info)
							continue
						
						if abi_begin:
							if 'ABI INFO END' in line:
								break
									
							with open(hpp_file, 'a') as hppf:
								if 'EXTERNAL_ABI_STRUCT' in line:
									external_struct_name = line[line.index('[')+1:line.index(']')]
									external_struct_lines = find_external_abi_struct(external_struct_name)
									#print '!!!!!!!!!!!!!!!!!!!!!!!!!!!external_struct_lines: ', external_struct_lines
									if len(external_struct_lines) > 0:
										for external_line in external_struct_lines:
											if 'char ' in external_line and '[' in external_line and ']' in external_line: #bytes
												external_line.replace('char ', 'bytes ')
												external_line[line.index('['):] = ';'
											hppf.write(external_line)
									continue
								
								if 'char ' in line and '[' in line and ']' in line: #bytes
									line = line.replace('char ', 'bytes ')
									line = line [0:line.index('[')]
									line += ';\n'
										
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
    	loop_dirs()
    	#print 'done'

