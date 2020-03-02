import hou
import hdefereval
import threading
import serial
import sys
from functools import partial
import time
import atexit

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
        
def exit_handler():
    hou.session.ser.write(("end").encode())
    hou.session.ser.close()
        
def mainWorker():
    com = "COM4"
    update_timer = time.time()
    atexit.register(exit_handler)
    display_reset_counter = 0
    
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
    # Increments: 0.01, 0.1, 1, 10, 100
    incr = 1.0
    while True:
        # get all Houdini nodes which are currently selected
        nodes = hou.selectedNodes()
        
        # read from serial til termination character '\n'
        input_line = hou.session.ser.readline() 
        
        # show the position of last selected node on an lcd screen
        if display_reset_counter > 10:
            first_turn_in_loop = False
            display_reset_counter = 0
            try:
                selected_node = nodes[-1]
                x_value = selected_node.parm(parm + 'x').eval()
                y_value = selected_node.parm(parm + 'y').eval()
                z_value = selected_node.parm(parm + 'z').eval()
                hou.session.ser.write((str(x_value) + "x" + str(y_value) + "y" + str(z_value) + "z").encode())
            except:
                pass
        else:
            if len(input_line) == 0:
                display_reset_counter += 1
            else:
                display_reset_counter = 0
        
        if len(input_line) > 0: 
            # change what movement to perform
            if input_line[0] == 's' or input_line[0] == 't' or input_line[0] == 'r':
                parm = input_line[0]
            # change increment for manipulation
            elif (input_line[0] == '1') or (input_line[0] == '1' and input_line[1] == '0') or (input_line[0] == '1' and input_line[1] == '0' and input_line[2] == '0') or (input_line[0] == '0' and input_line[1] == '.' and input_line[2] == '0' and input_line[3] == '1') or (input_line[0] == '0' and input_line[1] == '.' and input_line[2] == '1'):
                incr = float(input_line)
            # change movement along one axis or all
            else:
                if input_line[0] == 'x':
                    for node in nodes: 
                        try:
                            change = node.parm(parm + 'x').eval()
                            if input_line[1] == '-':
                                change -= incr
                                hdefereval.executeInMainThreadWithResult(partial(set_x, node, parm, change))
                            else:
                                change += incr
                                hdefereval.executeInMainThreadWithResult(partial(set_x, node, parm, change))
                        except:
                            pass
                elif input_line[0] == 'y':
                    for node in nodes:
                        try:
                            change = node.parm(parm + 'y').eval() 
                            if input_line[1] == '-':
                                change -= incr
                                hdefereval.executeInMainThreadWithResult(partial(set_y, node, parm, change))
                            else:
                                change += incr
                                hdefereval.executeInMainThreadWithResult(partial(set_y, node, parm, change))
                        except:
                            pass
                elif input_line[0] == 'z':
                    for node in nodes:  
                        try:
                            change = node.parm(parm + 'z').eval()
                            if input_line[1] == '-':
                                change -= incr
                                hdefereval.executeInMainThreadWithResult(partial(set_z, node, parm, change))
                            else:
                                change += incr
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
                                change_x -= incr
                                change_y -= incr
                                change_z -= incr
                                hdefereval.executeInMainThreadWithResult(partial(set_all, node, parm, change_x, change_y, change_z))
                            else:
                                change_x += incr
                                change_y += incr
                                change_z += incr
                                hdefereval.executeInMainThreadWithResult(partial(set_all, node, parm, change_x, change_y, change_z))
                        except:
                            pass
    
mainThread = threading.Thread(target=mainWorker)
mainThread.daemon = True
mainThread.start()
    
