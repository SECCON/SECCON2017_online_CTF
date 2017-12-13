from flask import Flask, render_template, request
import os
import hashlib

app = Flask(__name__)
#app.config['MAX_CONTENT_LENGTH'] = 1 * 1024 * 1024 * 10

@app.route('/')
def upload_file():
   return render_template('upload.html')
	
@app.route('/upload', methods = ['GET', 'POST'])
def upload_files():
   if request.method == 'POST':
      f1 = request.files['file1']
      data1 = f1.read()

      f2 = request.files['file2']
      data2 = f2.read()

      minLimit = 2017 * 1024
      maxLimit = 2018 * 1024

      data1_sha1 = hashlib.sha1(data1).hexdigest()
      data2_sha1 = hashlib.sha1(data2).hexdigest()

      data1_sha256 = hashlib.sha256(data1).hexdigest()
      data2_sha256 = hashlib.sha256(data2).hexdigest()

      size_flag = False
      sha1_flag = False
      sha256_flag = False

      if (minLimit < len(data1) < maxLimit) and (minLimit < len(data2) < maxLimit):
          size_flag = True
      if data1_sha1 == data2_sha1:
          sha1_flag = True
      if data1_sha256 != data2_sha256:
          sha256_flag = True

      if sha1_flag and sha256_flag and size_flag:
          return os.environ['flag']
      else:
          return "Wrong!!"

if __name__ == '__main__':
   app.run(debug = True)
