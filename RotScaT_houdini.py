import serial
import sys

def check_for_int(string_of_int):
    try:
        return int(string_of_int)
    except:
        print("Invalid input")
        print("Exiting...")
        hou.session.ser.close()
        sys.exit()

com = "COM4"
try:
    hou.session.ser = serial.Serial(com, 9600)
except:
    print("Serial set up on %s failed" %(com))
    print("Exiting...")
    hou.session.ser.close()
    sys.exit()

# Parameters: s == scale, t == transform and r == rotate
parm = "s"
while True:
    # read from serial til termination character '\n'
    input_line = hou.session.ser.readline()

    # get all Houdini nodes which are currently selected
    nodes = hou.selectedNodes()
    # idea: sent current values of the selected node back to the arduino
    # try:
    #     selected_node = nodes[-1]
    #     x_value = selected_node.parm(parm + 'x').eval()
    #     hou.session.ser.write(('x' + str(x_value) + '\n').encode())
    #     y_value = selected_node.parm(parm + 'y').eval()
    #     hou.session.ser.write(('y' + str(y_value) + '\n').encode())
    #     z_value = selected_node.parm(parm + 'z').eval()
    #     hou.session.ser.write(('z' + str(z_value) + '\n').encode())
    # except:
    #     pass

    # change what movement to perform
    if input_line == "s" or input_line == "t" or input_line == "r":
        parm = input_line
    # change movement along one axis or all
    else:
        if input_line[0] == 'x':
            change = check_for_int(input_line[1:])
            for node in nodes:
                try:
                    node.parm(parm + 'x').set(change)
                except:
                   pass
        elif input_line[0] == 'y':
            change = check_for_int(input_line[1:])
            for node in nodes:
                try:
                   node.parm(parm + 'y').set(change)
                except:
                    pass
        elif input_line[0] == 'z':
            change = check_for_int(input_line[1:])
            for node in nodes:
                try:
                    node.parm(parm + 'z').set(change)
                except:
                    pass
        else:
            change = check_for_int(input_line)
            for node in nodes:
                try:
                    node.parm(parm + 'x').set(change)
                    node.parm(parm + 'y').set(change)
                    node.parm(parm + 'z').set(change)
                except:
                    pass

# hou.session.ser.close() 
# muss ausgeführt werden, wenn jemand das Tool(Skript) schließt