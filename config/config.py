"""
This tool serves to create mini configuration profiles for the terminal.
"""
import json
import os

pathToSettings= os.environ['localappdata'] + '\\Packages\\Microsoft.WindowsTerminal_8wekyb3d8bbwe\\LocalState\\settings.json'


#72,132
ratio = 0.54

initialCols = 100
initialRows = 160
launchMode = "fullscreen"
#terminal_width = int(input("Width: "))
#terminal_height  = int(input("Height: "))

#s = input("fullscreen (y/n): ")
#if s == 'y':
#    fullscreen = True
#else:
#    fullscreen = False
simathProfile = {
    "colorScheme": "Color Scheme 17",
    "font": 
    {
        "size": 6
    },
    "guid": "{ee80e598-3f9c-4314-9495-170feb00b1e5}",
    "hidden": False,
    "icon": "\ue77b",
    "name": "simathCONFIG",
    "tabTitle": "SIMATH"
}

with open(pathToSettings,'r+') as json_data:
    config = json.load(json_data)
    #add profile if not existing
    if config['profiles']['list'][0]["name"] != "simathCONFIG":

        config['profiles']['list'] = [simathProfile] + config['profiles']['list']
    json_data.seek(0)        # <--- should reset file position to the beginning.
    json.dump(config, json_data, indent=4)
    json_data.truncate()     # remove remaining part
    json_data.close()
    
#execute SIMATH
#print()
os.system("")
command = "wt --size {width},{height} -F nt --startingDirectory {start}\\output\\x64\\Debug\\ -p simathCONFIG {start}\\output\\x64\\Debug\\simath.exe".format(width =initialCols, height = initialRows,start=os.getcwd())
os.system(command)
os.system('pause')