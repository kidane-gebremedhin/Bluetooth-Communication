#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int dynamic_bind_rc(int sock, struct sockaddr_rc *sockaddr, uint8_t *port)
{
    int err;
    for( *port = 2; *port <= 31; *port++ ) {
	printf("Tried Port %d\n", *port);
        sockaddr->rc_channel = *port;
        err = bind(sock, (struct sockaddr *)sockaddr, sizeof(sockaddr));
        if( ! err ) break;
    }
    if( port == 31 ) {
        err = -1;

    }
    return err;
}


int main(int argc, char **argv)
{

    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 111 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 0;
   
// bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

   dynamic_bind_rc(s, (struct sockaddr *)&loc_addr, &loc_addr.rc_channel);

    //get local address ?
    //~ ba2str( &loc_addr.rc_bdaddr, buf );
    //~ fprintf(stdout, "local %s\n", buf);

    // put socket into listening mode

    listen(s, 1);
    printf("Waiting for connections\n");
    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    fprintf(stderr, "accepted connection from\n");
    ba2str( &rem_addr.rc_bdaddr, buf );



	printf("\n%s\n", buf);
    memset(buf, 0, sizeof(buf));

	printf("\n%s\n", buf);
    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));

    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }

    // close connection
    close(client);
    close(s);
    return 0;
}
