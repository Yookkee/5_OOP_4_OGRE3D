Before building and running execute following commans:
setx OGRE_HOME "<path-to-OgreSDK-here>"

 If you are running Ogre 1.9 and are getting a runtime error saying: "Cannot locate resource shadows.glsl in resource group Popular or any...", you have to edit "resources_d.cfg" and "resources.cfg" and add "FileSystem=../../media/materials/programs/GLSL" to the files.