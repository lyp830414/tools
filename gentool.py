#!/usr/bin/env python

import os,sys,subprocess, time
import copy,shutil

struct_array     = []
abi_action_array = []
abi_table_container = []

def write_each_table(f, head_blank, table_dict, is_last_table):
	inside_blank = '	'
	
	f.write(head_blank + '{\n')
	f.write(head_blank + inside_blank + '\"table_name\": ' + '\"' + table_dict["table_name"] + '\",\n')
	f.write(head_blank + inside_blank + '\"index_type\": ' + '\"' + table_dict["index_type"] + '\",\n')
	f.write(head_blank + inside_blank + '\"key_names\": ' + ' [\n')
	f.write(head_blank + inside_blank*2 + '\"' + table_dict["key_names"] + '\"\n')
	f.write(head_blank + inside_blank + ' ],\n')
	f.write(head_blank + inside_blank + '\"key_types\": ' + ' [\n')
	f.write(head_blank + inside_blank*2 + '\"' + table_dict["key_types"] + '\"\n')
	f.write(head_blank + inside_blank + ' ],\n')
	f.write(head_blank + inside_blank + '\"type\": ' + '\"' + table_dict["table_type"] + '\"\n')
	
	if is_last_table:
		f.write(head_blank + '}\n')
	else:
		f.write(head_blank + '},\n')

def write_each_action(f, head_blank, action_dict, is_last_action):
	inside_blank = '	'
	
	f.write(head_blank + '{\n')
	f.write(head_blank + inside_blank + '\"action_name\": ' + '\"' + action_dict["action_name"] + '\",\n')
	f.write(head_blank + inside_blank + '\"type\": ' + '\"' + action_dict["action_type"] + '\"\n')
	if is_last_action:
		f.write(head_blank + '}\n')
	else:
		f.write(head_blank + '},\n')
		

def write_each_struct(f, head_blank, struct_info, is_last_struct):
	
	inside_blank = '	'
	last_info_line_no = len(struct_info)
	curr_line_no = 0
		
	for each_line in struct_info:
		curr_line_no += 1
		if 'struct ' in each_line and '{' in each_line:
			extra_info = each_line[each_line.find('{'):].strip()
			if len(extra_info) > 1:
				print 'Error! Please check your abi file, struct do not allow any extra info( except blanks ) after \'stuct {\' '
				exit(1)
			
			struct_name = each_line[each_line.find('struct ') + len('struct ') : each_line.find('{')].strip()
			f.write(head_blank + '{\n')
			f.write(head_blank + inside_blank + '\"name\": ' + '\"' + struct_name + '\",\n')
			f.write(head_blank + inside_blank + '\"base\": \"\",\n')
			f.write(head_blank + inside_blank + '\"fields\": {\n')
			continue
		
		if '};' in each_line:
			extra_info = each_line[each_line.find('};') + len('};') : ].strip()
			if len(extra_info) > 0:
				print 'Error! Please check your abi file, struct do not allow any extra info( except blanks ) after \'}; in each struct definition\' '
				exit(1)
			f.write(head_blank + '      ' + '}\n')
			if is_last_struct:
				f.write(head_blank + inside_blank + '}\n')
			else:
				f.write(head_blank + inside_blank + '},\n')
			return #struct is end.		
	
		#here are struct body parts=>body inside of '{' and '}'.
		val_list = each_line[ 0 : each_line.find(';')].strip().split()
		
		valname = ''
		valtype = ''
		for item in val_list:
			if item.strip():
				if not valtype:
					valtype = item.strip()
					if valtype in ('uint8_t', 'int8_t', 'int8', 'uint8', 'char_t', 'char8'):
						valtype = 'uint8'
					elif valtype in ('uint16_t', 'int16_t', 'int16', 'uint16'):
						valtype = 'uint16'
					elif valtype in ('uint32_t', 'int32_t', 'int32', 'uint32', 'int', 'uint'):
						valtype = 'uint32'
					elif valtype in ('uint64_t', 'int64_t', 'int64'):
						valtype = 'uint64'
				elif not valname:
					valname = item.strip()
					if '[' in valname and ']' in valname:
						if valtype in ('char', 'string'):
							valtype = 'string'
						else:
							valtype = 'bytes'
						valname = valname[0:valname.index('[')]
					break

		inside_blank_for_struct = inside_blank * 2
		if curr_line_no == last_info_line_no - 1:
			f.write(head_blank + inside_blank_for_struct + '\"' + valname + '\": ' + '\"' + valtype + '\"\n')
		else:
			f.write(head_blank + inside_blank_for_struct + '\"' + valname + '\": ' + '\"' + valtype + '\",\n')


def gen_abi_fileEx(abi_path):
	#overwrite mode
	with open(abi_path, 'w') as f:
		f.write('{\n')
		#gen types
		f.write('	' + '\"types\": [],\n')
		
		#gen abi structs
		f.write('	' + '\"structs\": [\n')
		last_struct_no = len(struct_array)
		curr_struct_no = 0
		for each_struct in struct_array:
			curr_struct_no += 1
			if curr_struct_no == last_struct_no:
				write_each_struct ( f, '              ', each_struct, True )
			else:	
				write_each_struct ( f, '              ', each_struct, False )
		
		f.write('       ' + '],\n')
		
		#gen abi actions
		f.write('	' + '\"actions\": [\n')
		last_action_no = len(abi_action_array)
		curr_action_no = 0
		for each_action in abi_action_array:
			curr_action_no += 1
			if curr_action_no == last_action_no:
				write_each_action ( f, '              ', each_action, True )
			else:	
				write_each_action ( f, '              ', each_action, False )
		f.write('       ' + '],\n')
		
		#gen abi tables
		f.write('	' + '\"tables\": [\n')
		last_table_no = len(abi_table_container)
		curr_table_no = 0
		for each_table in abi_table_container:
			curr_table_no += 1
			if curr_table_no == last_table_no:
				write_each_table ( f, '              ', each_table, True )
			else:	
				write_each_table ( f, '              ', each_table, False )
		f.write('       ' + ']\n')
		
		f.write('}\n')

def check_virables():
	if len(struct_array) <= 0:
		print 'Error! No struct is defined in your abi file!'
		exit(1)
	if len(abi_action_array) <= 0:
		print 'Error! No abi action is defined in your abi file!'
		exit(1)

def gen_bto_abiEx(hpp_path):
	index_type = ''
        index_type_value = ''

        key_names = ''
        key_names_value = ''

  	key_types = ''
        key_types_value = ''

	
	struct_lines     = []
	abi_action_lines = []
	abi_table_lines  = []	

	
	abi_table_array  = []
         
	struct_start     = False
	abi_action_start = False
	abi_table_start  = False
	
	try:
		with open(hpp_path) as f:
			for line in f.readlines():
				if '@abi table ' in line:
					abi_table_name = line[(line.find('@abi table ') + len('@abi table ')) : line.find(':')].strip()
					
					if line.find('index_type') < 0 \
					or line.find('key_names') < 0 \
                                        or line.find('key_types') < 0 \
					or line.find('[') < 0 \
					or line.find(']') < 0 :
						print '\nError! \'@abi tale\' section should include [index_type:***, key_names:***, key_types:***].\n'
						exit(1)
					line = line.replace(' ','')
					
					line_sp = line[line.index('[')+1:line.index(']')].split(',')
					for item in line_sp:
						if 'index_type' in item:
							index_type, index_type_value = item.split(':')
							if len(index_type_value) <= 0:
								print '\nError! \'index_type\' has no value!\n'
						elif 'key_names' in item:
							key_names, key_names_value = item.split(':')
							if len(key_names_value) <= 0:
								print '\nError! \'key_names\' has no value!\n'
						elif 'key_types' in item:
							key_types, key_types_value = item.split(':')
							if len(key_types_value) <= 0:
								print '\nError! \'key_types\' has no value!\n'
					
					abi_table_start = True
					struct_start = True
					continue
				elif '@abi action ' in line:	
					abi_action_name = line[(line.find('@abi action ') + len('@abi action ')) : ].strip()
					 
					if len(abi_action_name) <= 0:
						print '\nError! \'@abi action\' should not be none!'
						exit(1)
						
					abi_action_start = True
					struct_start = True
					
					continue
				elif 'struct ' in line and '{' in line:
					if abi_action_start is False and abi_table_start is False:
						struct_start = True
						struct_lines.append(line)
						continue
				
				if struct_start is True:
					if not line.strip():
						continue
					struct_lines.append(line)
	 				
					if abi_action_start is True:
						abi_info = {}
						abi_info["action_name"] = abi_action_name
						
                                                abi_action_type = line.replace('struct ', '')
						abi_action_type = abi_action_type.replace('{', '').strip()

						abi_info["action_type"] = abi_action_type
						abi_action_array.append(abi_info)
	
						abi_action_start = False
						abi_action_lines = []
						
					elif abi_table_start is True:
						
                                                abi_table_type = line.replace('struct ', '')
						abi_table_type = abi_table_type.replace('{', '').strip()
						
						abi_table_array_map = dict()
						abi_table_array_map["table_name"] = abi_table_name
						abi_table_array_map["index_type"] = index_type_value 
						abi_table_array_map["key_names"]  = key_names_value
						abi_table_array_map["key_types"]  = key_types_value
						abi_table_array_map["table_type"] = abi_table_type
						
						abi_table_container.append(abi_table_array_map)
						
						abi_table_lines = []
						index_type_value = ''
						key_names_value = ''
						key_types_value = ''
						abi_table_array = []
							
						abi_table_start = False
						abi_table_lines = []
						
						
					if '};' in line:
						
						struct_array.append(struct_lines)
						struct_lines = []
							
						if abi_action_start is True:
							pass
						elif abi_table_start is True:
							pass
							
						struct_start = False
						
						
	except Exception as error:
		s=sys.exc_info()
        	print "Error '%s' happened on line %d" % (s[1],s[2].tb_lineno)
		print 'Error! Process file %s failed: error: %s' %(hpp_path, error)
		return

def help_info():
	print '\n******GENERATE ABI *******\n'
	print 'Parameter: ***.hpp\n'
	print 'Output: ***.abi\n'
	print 'Example: \n\
    python genabi.py assetmng.hpp\n\
    then the [assetmng.abi] will ben generated under the current directory.\n'

	print '\n******GENERATE WASM *******\n'
	print 'Parameter: wasm ***.cpp\n'
	print 'Output: ***.wasm\n'
	print 'Example: \n\
    python genabi.py wasm assetmng.cpp\n\
    then the [assetmng.wasm] will ben generated under the current directory.\n'
	print 'Restrictions for generate wasm file\'s cpp \n\n\
    a. Your cpp file must #include "lib/contractcomm.hpp" at first.\n\
    b. Be sure to keep "lib" folder directly under your cpp file\'s path.\n\
    c. Your cpp file must be under the directory which contains gentool.py\n\
    d. If you move the \'lib\' directory onto other path, it even still reports failure as you change the include lib path in your cpp\n'

def excute_cmd(cmd):

        P =subprocess.Popen(cmd, shell=True)
        P.communicate()

def gen_wasm(cpp_filename):
	if not os.path.exists('clang') and not os.path.exists('llc') and not os.path.exists('llvm-link') and not sos.path.exists('s2wasm'):
		print 'Error! Please keep these files [clang | llc | llvm-link | s2wasm] under current directory.'
		exit(1)
	if not os.path.exists(cpp_filename):
		print 'Error! File %s does not exist.' %cpp_filename
		exit(1)
		
	tmpdir = './tmpdir'
	builddir = './tmpdir/build'
	
	if os.path.isdir(tmpdir):
		shutil.rmtree(tmpdir)

	os.mkdir(tmpdir)
	os.mkdir(builddir)

	libdir = './lib'
	
	if not os.path.isdir(libdir):
		print 'Error! Could not find [lib] directory in current directory.'
		exit(1)
	
	gen_build_cmd = './clang -emit-llvm -O3 --std=c++14 --target=wasm32 -ffreestanding -nostdlib -fno-threadsafe-statics -fno-rtti -fno-exceptions -I %s -I . -c %s -o %s/%s' %(libdir, cpp_filename, builddir, cpp_filename)
	print gen_build_cmd
	excute_cmd(gen_build_cmd)
	
	gen_link_cmd = './llvm-link -o %s/linked.bc %s/*' %(tmpdir, builddir)
	print gen_link_cmd
	excute_cmd(gen_link_cmd)
	
	gen_assembly_cmd = './llc --asm-verbose=false -o %s/assembly.s %s/linked.bc' %(tmpdir, tmpdir)
	print gen_assembly_cmd
	excute_cmd(gen_assembly_cmd)
	
	wast_filename = cpp_filename[:-4] + '.wast'
	gen_wast_cmd = './s2wasm -o %s -s 16384 %s/assembly.s' %(wast_filename, tmpdir)
	print gen_wast_cmd
	excute_cmd(gen_wast_cmd)
	
	wasm_filename = cpp_filename[:-4] + '.wasm'
	gen_wasm_cmd = './wat2wasm %s -o %s' %(wast_filename, wasm_filename)
	excute_cmd(gen_wasm_cmd)
	
	if os.path.isdir(tmpdir):
		shutil.rmtree(tmpdir)
	
if __name__ == '__main__':
	if len(sys.argv) > 1:
		if sys.argv[1] == 'wasm':
			if len(sys.argv) != 3:
				help_info()
				exit(1)
			if not os.path.exists(sys.argv[2]):
				print 'Error! File %s not exist!' %sys.argv[2]
				exit(1)
			if not '.cpp' in sys.argv[2]:
				print 'Error! %s is not a cpp file!' %sys.argv[2]
				exit(1)
		
			gen_wasm(sys.argv[2])
			exit(0)
	
	if len(sys.argv) <= 1 or len(sys.argv) > 2 or sys.argv[1].lower() == 'help':
		help_info()
		exit(0)
	
	if not '.hpp' in sys.argv[1]:
		help_info()
		exit(1)
	
	if not os.path.exists(sys.argv[1]):
		print 'Error! File %s not exist!' %sys.argv[1]
		exit(1)

	
	abi_output = sys.argv[1][ : sys.argv[1].find('.hpp')] + '.abi'
	
	gen_bto_abiEx(sys.argv[1])
	check_virables()
	gen_abi_fileEx(abi_output)
	
	
	print '\n[ %s ] is generated. Please have a check.\n' %abi_output
	exit(0)
  	
	#print '\n\n====>DONE! struct_array:\n'
	for each_struct in struct_array:
		for each_line in each_struct:
			print each_line

  	#print '\n\n====>DONE! abi_action_array:\n'
	for each_abi_struct in abi_action_array:
		for each_abi_line in each_abi_struct:
			print each_abi_line
		
  	#print '\n====>DONE! abi_table_container:\n', abi_table_container
	#print 'TRY PRINT:'
	for each_abitable_map in abi_table_container:
		#print '\n|+++++++++++++++++++++++++++++++++|\n'
		for each_item in each_abitable_map.items():
			if 'table_lines' in each_item[0]:
				#print '\n(table_lines: \n'
				for each_table_struct in each_item[1]:
					for each_table_line in each_table_struct:
						print each_table_line,'\n'
				#print ')'
			else:
				print each_item 
