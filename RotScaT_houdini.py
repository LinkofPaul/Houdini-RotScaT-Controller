import hou
import hdefereval
import threading
import serial
import sys
from functools import partial
import time

def set_x(node, parm, change):
    node.parm(parm + 'x').set(change)

def set_y(node, parm, change):
    node.parm(parm + 'y').set(change)

def set_z(node, parm, change):
    node.parm(parm + 'z').set(change)

def set_all(node, parm, change_x, change_y, change_z):
    node.parm(parm + 'x').set(change_x)
    node.parm(parm + 'y').set(change_y)
    node.parm(parm + 'z').set(change_z)

def updateDisplay(nodes, parm):
    try:
        selected_node = nodes[-1]
        x_value = selected_node.parm(parm + 'x').eval()
        y_value = selected_node.parm(parm + 'y').eval()
        z_value = selected_node.parm(parm + 'z').eval()
        hou.session.ser.write((str(x_value) + "x " + str(y_value) + "y " + str(z_value) + "z " ).encode())
    except:
        pass

def mainWorker():
    com = "COM4"
    update_timer = time.time()
    
    try:
        hou.session.ser = serial.Serial(com, 9600, timeout=0.2)
    except serial.SerialException as e:
        if str(e).find('Access is denied') != -1:
            pass
        else:
            print("Serial set up on %s failed" %(com))
            sys.exit()
    except:
        print("Serial set up on %s failed" %(com))
        sys.exit()
    
    # Parameters: s == scale, t == transform and r == rotate
    parm = "t"

    while True:
        # get all Houdini nodes which are currently selected
        nodes = hou.selectedNodes()
        
        if (time.time()-update_timer) > 4.0:
            updateDisplay(nodes, parm)
            update_timer = time.time()
            
        # read from serial til termination character '\n'
        input_line = hou.session.ser.readline() 
                    
        if len(input_line) > 0: 
            # change what movement to perform
            if input_line == "s" or input_line == "t" or input_line == "r":
                parm = input_line
            # change movement along one axis or all
            else:
                if input_line[0] == 'x':
                    for node in nodes: 
                        try:
                            change = node.parm(parm + 'x').eval()
                            if input_line[1] == '-':
                                change -= 1
                                hdefereval.executeInMainThreadWithResult(partial(set_x, node, parm, change))
                            else:
                                change += 1
                                hdefereval.executeInMainThreadWithResult(partial(set_x, node, parm, change))
                        except:
                            pass
                elif input_line[0] == 'y':
                    for node in nodes:
                        try:
                            change = node.parm(parm + 'y').eval() 
                            if input_line[1] == '-':
                                change -= 1
                                hdefereval.executeInMainThreadWithResult(partial(set_y, node, parm, change))
                            else:
                                change += 1
                                hdefereval.executeInMainThreadWithResult(partial(set_y, node, parm, change))
                        except:
                            pass
                elif input_line[0] == 'z':
                    for node in nodes:  
                        try:
                            change = node.parm(parm + 'z').eval()
                            if input_line[1] == '-':
                                change -= 1
                                hdefereval.executeInMainThreadWithResult(partial(set_z, node, parm, change))
                            else:
                                change += 1
                                hdefereval.executeInMainThreadWithResult(partial(set_z, node, parm, change))
                        except:
                            pass
                elif input_line[0] == 'a':
                    for node in nodes:  
                        try:
                            change_x = node.parm(parm + 'x').eval()
                            change_y = node.parm(parm + 'y').eval()
                            change_z = node.parm(parm + 'z').eval()
                            if input_line[1] == '-':
                                change_x -= 1
                                change_y -= 1
                                change_z -= 1
                                hdefereval.executeInMainThreadWithResult(partial(set_all, node, parm, change_x, change_y, change_z))
                            else:
                                change_x += 1
                                change_y += 1
                                change_z += 1
                                hdefereval.executeInMainThreadWithResult(partial(set_all, node, parm, change_x, change_y, change_z))
                        except:
                            pass
    
mainThread = threading.Thread(target=mainWorker)
mainThread.daemon = True
mainThread.start()
    
 