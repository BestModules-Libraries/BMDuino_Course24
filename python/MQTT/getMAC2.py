from wsgiref.simple_server import make_server
import uuid, re

def wsginter(environ, start_response):

    status = '200 OK'
    headers = [('Content-type', 'text/plain; charset=utf-16')]
    start_response(status, headers)

    macs = ':'.join(re.findall('..', '%012x' % uuid.getnode()))

    return [str(macs).encode('utf-16')]

# Syntax Objective -> start the test server and trigger the function intermediary to wsgiref
with make_server('', 8000, wsginter) as httpd:

    print(f'Serving on port 8000...')
    # Serve until process is killed
    httpd.serve_forever()