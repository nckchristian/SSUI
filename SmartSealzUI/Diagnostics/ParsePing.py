import subprocess

address == '192.168.0.1'

def ping(ip):
    ping_command = ['ping', ip, '-c 1']
    shell_needed = False

    ping_output = subprocess.run(ping_command, shell = False, stdout=subprocess.PIPE)
    success = ping_output.returncode
    return 1 if success ==0 else 0

out = ping(address)
print(out)
