# CPE476Lab1

<h1>How to use git</h1>
<code>git add . --all</code>  
<code>git commit -m "message"</code>  
<code>git fetch</code>  
<code>git rebase origin/master</code>  
 &nbsp;&nbsp;&nbsp;&nbsp;  While there are merge conflicts:  
 &nbsp;&nbsp;&nbsp;&nbsp;  modify code manually until it is working properly  
 &nbsp;&nbsp;&nbsp;&nbsp;  <code>git add . --all</code>  
 &nbsp;&nbsp;&nbsp;&nbsp;  <code>git commit -m "message"</code>  
 &nbsp;&nbsp;&nbsp;&nbsp;  <code>git rebase --continue</code>  
<code>git push</code>  

<h1>Cross-Platform Building</h1>
This project supports janky builds on macs, linux, and the CSL machines.<br>
<b>IF YOU WANT TO CHANGE includes.h BE SURE TO MODIFY YOUR RESPECTIVE INCLUDES FILE.</b><br>
For instance, if you are on a CSL machine, change includes_csl.h.<br>
<br>
To build the standard build, run<br>
&nbsp;&nbsp;&nbsp;&nbsp; <code>make</code><br>
To build the CSL build, run<br>
&nbsp;&nbsp;&nbsp;&nbsp; <code>make csl</code><br>

<h1>Creating the path variable for freetype2:</h1>
First install freetype <a href="http://www.linuxfromscratch.org/blfs/view/svn/general/freetype2.html">here</a>
<br>Find the path:<br>
&nbsp;&nbsp;&nbsp;&nbsp; <code>freetype-config --cflags</code>
<br>Copy the path in the output<br>
<br>Add line into ~/.profile like this:<br>
&nbsp;&nbsp;&nbsp;&nbsp; <code>export FREETYPE_DIR=paste here</code>
<br>Adding this line into ~/.profile will run the export command every time you open a new terminal.<br>
<br>Without restarting your terminal, or if things aren't working, try:<br>
&nbsp;&nbsp;&nbsp;&nbsp; <code>source ~/.profile</code>
<br>Makefile should work at this point<br>

<h1>Creating the path variables for irrKlang:</h1>
<br>I think you have to manually coppy the irrKlang header files into wherever your computer keeps them, sorry :( For ubuntu this is /usr/include/ The other thing we could do is just pull it from the irrKlang directory that has to be in there anyway.</br>
<br>Add the following lines into ~/.profile:<br>
&nbsp;&nbsp;&nbsp;&nbsp; <code>export IRRKLANG_DIR=irrKlang-64bit-1.5.0/bin/linux-gcc-64/libIrrKlang.so</code>
<code>export IRRKLANGFLAC_DIR=irrKlang-64bit-1.5.0/bin/linux-gcc-64/ikpFlac.so</code>
<code>export IRRKLANGMP3_DIR=irrKlang-64bit-1.5.0/bin/linux-gcc-64/ikpMP3.so</code>
<br>Adding this line into ~/.profile will run the export command every time you open a new terminal.<br>
<br>Without restarting your terminal, or if things aren't working, try:<br>
&nbsp;&nbsp;&nbsp;&nbsp; <code>source ~/.profile</code>
<br>Makefile should work at this point<br>

<h1>Issue Tracking</h1>
<a href="https://bitbucket.org/japanesefestival/cpe476-final-project/issues?status=new&status=open">Go to BitBucket</a>
