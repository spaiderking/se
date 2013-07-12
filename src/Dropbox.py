'''
Created on May 1, 2013

@author: Fahad
'''

import sys
import cStringIO
from dropbox import client, rest, session
# Include the Dropbox SDK libraries

class DropboxAPI:
	client = __import__('dropbox.client')
	def __init__(self):
		# from dropbox import client, rest, session
		# Get your app key and secret from the Dropbox developer website
		APP_KEY = 'krrlaf6p6alrvwp'
		APP_SECRET = 'wi5o59b5rwao11o'

		# ACCESS_TYPE should be 'dropbox' or 'app_folder' as configured for your app
		ACCESS_TYPE = 'dropbox'

		sess = session.DropboxSession(APP_KEY, APP_SECRET, ACCESS_TYPE)

		request_token = sess.obtain_request_token()

		url = sess.build_authorize_url(request_token)

		# Make the user sign in and authorize this token
		print "url:", url
		print "Please visit this website and press the 'Allow' button, then hit 'Enter' here."

		import webbrowser
		new = 2 # open in a new tab, if possible
		# open a public URL, in this case, the webbrowser docs
		webbrowser.open(url,new=new)

		raw_input()
		
		# This will fail if the user didn't visit the above URL and hit 'Allow'
		access_token = sess.obtain_access_token(request_token)

		DropboxAPI.client = client.DropboxClient(sess)	
		print "linked account:", DropboxAPI.client.account_info()
	
	def upload(self, filename, fileContents):
		import filelike
		from StringIO import StringIO as SIO
		f = filelike.join([SIO(fileContents)])
		response = DropboxAPI.client.put_file('/%s' %filename, f)
		print "uploaded:", response

		folder_metadata = DropboxAPI.client.metadata('/')
		# print "metadata:", folder_metadata
		print "Uploaded\n"
		return;

	def download(self, filename, fileContents):
		f, metadata = DropboxAPI.client.get_file_and_metadata('/%s' %filename)
		# print(metadata)
		return f.read();
