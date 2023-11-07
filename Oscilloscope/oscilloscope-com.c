#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "error: not enough arguments\n");
        return -1;
    }

    char *dev = argv[1];
    int   baud_rate;

    sscanf(argv[2], "%i", &baud_rate);

    switch (baud_rate) {
        case 50:
            baud_rate = B50;
            break;
        case 75:
            baud_rate = B50;
            break;
        case 110:
            baud_rate = B110;
            break;
        case 134:
            baud_rate = B134;
            break;
        case 150:
            baud_rate = B150;
            break;
        case 200:
            baud_rate = B200;
            break;
        case 300:
            baud_rate = B300;
            break;
        case 600:
            baud_rate = B600;
            break;
        case 1200:
            baud_rate = B1200;
            break;
        case 1800:
            baud_rate = B1800;
            break;
        case 2400:
            baud_rate = B2400;
            break;
        case 4800:
            baud_rate = B4800;
            break;
        case 9600:
            baud_rate = B9600;
            break;
        case 19200:
            baud_rate = B19200;
            break;
        case 38400:
            baud_rate = B38400;
            break;
        case 57600:
            baud_rate = B57600;
            break;
        case 115200:
            baud_rate = B115200;
            break;
        default:
            fprintf(stderr, "error: incorrect baud rate\n");
            break;
    }

    int fd = open(dev, O_RDWR | O_NOCTTY);

    if(fd < 0) {
        fprintf(stderr, "error: cannot open %s\n", dev);
        return -1;
    }

    struct termios com;

    // Get the current attributes of the Serial port
    tcgetattr(fd, &com);

    cfsetispeed(&com, baud_rate);
    cfsetospeed(&com, baud_rate);

    /* 8N1 Mode */
    com.c_cflag &= ~PARENB; // Disables the Parity Enable bit(PARENB)
    com.c_cflag &= ~CSTOPB; // CSTOPB = 2 Stop bits, here it is cleared so 1 Stop bit
    com.c_cflag &= ~CSIZE;	// Clears the mask for setting the data size
    com.c_cflag |=  CS8;    // Set the data bits = 8

    com.c_cflag &= ~CRTSCTS;       // No Hardware flow Control
    com.c_cflag |= CREAD | CLOCAL; // Enable receiver,Ignore Modem Control lines

    com.c_iflag &= ~(IXON | IXOFF | IXANY);         // Disable XON/XOFF flow control both i/p and o/p
    com.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Non Cannonical mode
    com.c_oflag &= ~OPOST;                          // No Output Processing

    com.c_cc[VMIN]  = 40; // Read at least 40 characters
    com.c_cc[VTIME] = 10; // Wait indefinetly

    if((tcsetattr(fd,TCSANOW,&com)) != 0)
        fprintf(stderr, "error: cannot configure %s\n", dev);

    char data_buffer[255];
    char read_buffer[255];
    int  bytes_read  = 0;
    int  data_length = 0;

    FILE *data = fopen("history.csv", "w");
    int   rows = 0;

    FILE *gnuplot = popen("gnuplot -", "w");
    fprintf(gnuplot, "set datafile separator \",\";");
    fprintf(gnuplot, "plot './history.csv' u 1:2 w l title 'teplota [°C]', "
                     "     './history.csv' u 1:3 w l title 'rel. vlhkosť [%%]', "
                     "     './history.csv' u 1:($4/10) w l title 'tlak vzduchu [hPa] / 10', "
                     "     './history.csv' u 1:5 w l title 'relatívna výška od zeme [m]';\n");

    while (1) {
        tcflush(fd, TCIFLUSH);
        bytes_read = read(fd, &read_buffer, 255);

        for(int i = 0; i < bytes_read; i++) {
            if (read_buffer[i] == '\n') {
                data_buffer[data_length++] = '\0';
                
                if (data_length > 2) {
                   // printf("%s\n", data_buffer + 1);
                    fprintf(data, "%i", rows);

                    float cell[4];
                    for (int j = 0, k = 0; j < data_length && k < 4; j++) {
                        float cell;

                        if (data_buffer[j] == ':') {
                            sscanf(&(data_buffer[j + 1]), "%f", &cell);
                            fprintf(data, ", %f", cell);
                            k++;
                        }
                    }
                    fprintf(data, "\n");
                    fflush(data);
                    rows++;

                    fprintf(gnuplot, "replot\n");
                    fflush(gnuplot);
                }

                data_length = 0;
            }

            data_buffer[data_length++] = read_buffer[i];

            if (data_length >= 255)
                data_length = 0;
        }

    }

    fclose(gnuplot);
    fclose(data);
    close(fd);
    return 0;
}

