What are the prerequisites for this app to work? 
1. Have GIT installed on your computer. If you do not have it installed or are unsure, this link provides the latest official installer: https://git-scm.com/downloads/win.
2. Have a GitHub account.
3. If you do not know how to use git, I highly recommend downloading the GUI provided by GitHub: https://github.com/apps/desktop. I will use this GUI in the instructions later on, but feel free to skip this step if you prefer using the terminal.
4. Be on the Windows operating system. Unfortunately, for now, I lack the knowledge to fully integrate this app on Linux, but it is my next goal.

Now, assuming you meet all of the prerequisites, let me show you how to set up the Auto backup app.

First Open Steam and navigate to the Library tab, find ATLYSS and right-click it. Then from the popup select "Manage", hover over "Browse local files" and select it.

![Screenshot_43](https://github.com/user-attachments/assets/392781a8-9c32-41cf-ad4f-b81367dbce44)

A new window of File Explorer will open. Copy the path to that folder. You can do so by clicking on the empty space in the path window. A path will be automatically selected. Ctrl+C will do the trick. Keep it in the background for now.

![image](https://github.com/user-attachments/assets/7d0fef9c-3923-478d-86ee-5f656408f99f)


Next, open the GitHub Desktop app. Click "File" and then "New Repository".

![image](https://github.com/user-attachments/assets/4c1c5d5d-7510-412a-a306-918a8af9bdca)

A new window will pop up. Name the repository "ATLYSS".
In the "Local Path" window paste the previously copied path to the Atlyss folder except delete the "/ATLYSS" part.
The rest can be left as default or empty. Then click "Create repository".
How it should look like:

![image](https://github.com/user-attachments/assets/6f313107-4f2a-4a15-8d30-79d5c7b50496)

In the GitHub Desktop, you will now see a big blue button "Publish repository". Once pressed a new pop-up will appear.
Name the remote repository however you like, it Doesn't have to be named ATLYSS (I advise naming it something like "ATLYSS Backup" or "Atlyss Failsafe" or something along those lines).
Remember to have "Keep this code private" checked.
Then press "Publish repository".
How it should look like:

![image](https://github.com/user-attachments/assets/c35da75c-dad9-4fa6-9011-59372959ea6f)

The last few steps are to download Atlyss_auto_git_v_1_0.zip file from the Reliese tab on this webpage. Once downloaded move the Atlyss_auto_git_v_1_0.zip file into the ATLYSS folder and extract it there. Move the content inside the Atlyss_auto_git_v_1_0 folder (Atlyss_auto_git.exe and appicon.ico) to the ATLYSS one. You can delete the Atlyss_auto_git_v_1_0.zip and Atlyss_auto_git_v_1_0 folders. 

All done. To play ATLYSS simply run the Atlyss_auto_git.exe and all the backupping will happen in the background :)

One thing that I adive is to make a shortcut of the Atlyss_auto_git.exe and move it to the desctop. You can rename the shortcut and change it's icon to appicon.ico for a nicer experience.
