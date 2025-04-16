#!/usr/bin/env python3

# -----------------------------------------------------------------
# openfortivpn configurator - command line tools to generate 
#                             configuration file for openfortivpn
#                             to connect to Fortine  VPNs  
# Copyright (C) 2025  Gabriele Bonacini
#
# This program is free software for no profit use; you can redistribute
# it and/or modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
# -----------------------------------------------------------------


import getopt
import sys
assert sys.version_info >= (3, 0, 0), "Python 3 is required."
import os
import subprocess
import re
from getpass import getpass


def getParams():
	global ip
	global port
	global usr
	global pwd
	global cmdName
	cmdName = sys.argv[0]
	argv = sys.argv[1:] 

	try:
		opts, args = getopt.getopt(argv, "i:p:u:") 
		for opt, arg in opts: 
			if opt in ['-i']: 
				ip = arg 
			elif opt in ['-p']: 
				port = arg 
			elif opt in ['-u']: 
				usr = arg 

		if ip == None or port == None or usr == None:
			return False

		print("Insert Password:", file=sys.stderr)
		pwd = getpass()

		return True
	except:
		return False

def getCertificateSha256():
	global certificateSha246
	sslCmd = "echo | openssl s_client -connect " + ip + ":" + port + " 2>/dev/null | openssl x509 -outform der | sha256sum"
	try:
		ostream = os.popen(sslCmd)
		certificateSha246 = ostream.read()
		ostream.close()
		certificateSha246 = re.sub(r'[ ].*$', '', certificateSha246)
		return True
	except:
		return False

def main():
	if getParams() != True:
		print("ERROR: Wrong parameters", file=sys.stderr)
		syntax = "{} -i <ip> -p <port> -u <username>\nInsert the password when requested.\n".format(cmdName)
		print("\nSyntax: ", file=sys.stderr)
		print(syntax, file=sys.stderr)
		exit(1)

	print("-> Using following user:", file=sys.stderr)
	print(usr, file=sys.stderr)
	print("-> Using following ip and port:", file=sys.stderr)
	print(ip, file=sys.stderr)
	print(port, file=sys.stderr)

	if getCertificateSha256() != True:
		print("ERROR: Can't get Certificate", file=sys.stderr)
		exit(1)

	print("-> Calculated following SHA56 hash from certificate:", file=sys.stderr)
	print(certificateSha246, file=sys.stderr)
	print("\n\n", file=sys.stderr)

	configuration = "host = {}\nport = {}\nusername = {}\npassword = {}\ntrusted-cert = {}\n".format(ip, port, usr, pwd, certificateSha246)
	print(configuration)

	exit(0)

if __name__=="__main__":
	main()
