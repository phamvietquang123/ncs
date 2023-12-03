# Makefile for server.c and client.c

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Directories
FTP_SRC_DIR = ftp
SMTP_SRC_DIR = smtp
BUILD_DIR = executable

# Targets
all: $(BUILD_DIR)/ftp_server $(BUILD_DIR)/ftp_client $(BUILD_DIR)/smtp_server $(BUILD_DIR)/smtp_client

# Create the build directory
$(BUILD_DIR):
	mkdir -p $@

# Compile ftp_server.c
$(BUILD_DIR)/ftp_server: $(FTP_SRC_DIR)/server.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Compile ftp_client.c
$(BUILD_DIR)/ftp_client: $(FTP_SRC_DIR)/client.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Compile smtp_server.c
$(BUILD_DIR)/smtp_server: $(SMTP_SRC_DIR)/server.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Compile smtp_client.c
$(BUILD_DIR)/smtp_client: $(SMTP_SRC_DIR)/client.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Clean up generated files
clean:
	rm -rf $(BUILD_DIR)

# Target for building only ftp_server
ftp_server: $(BUILD_DIR)/ftp_server

# Target for building only ftp_client
ftp_client: $(BUILD_DIR)/ftp_client

# Target for building both ftp_server and ftp_client
ftp: ftp_server ftp_client

# Target for building only smtp_server
smtp_server: $(BUILD_DIR)/smtp_server

# Target for building only smtp_client
smtp_client: $(BUILD_DIR)/smtp_client

# Target for building both smtp_server and smtp_client
smtp: smtp_server smtp_client
