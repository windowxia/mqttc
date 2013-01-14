/* 
**
** packet.h - mqtt packet encode and decode header
**
** Copyright (c) 2013  Ery Lee <ery.lee at gmail dot com>
** All rights reserved.
**
*/
#ifndef __MQTT_PACKET_H
#define __MQTT_PACKET_H

typedef unsigned int bool;

typedef union _Header
{
	char byte;	/**< the whole byte */
	struct
	{
		bool retain : 1;		/**< retained flag bit */
		unsigned int qos : 2;	/**< QoS value, 0, 1 or 2 */
		bool dup : 1;			/**< DUP flag bit */
		unsigned int type : 4;	/**< message type nibble */
	} bits;
} Header;


typedef	union _ConnFlags {
	unsigned char byte;	/**< all connect flags */
	struct
	{
		int : 1;				/**< unused */
		bool cleansess : 1;	/**< cleansession flag */
		bool will : 1;			/**< will flag */
		unsigned int willqos : 2;	/**< will QoS value */
		bool willretain : 1;		/**< will retain setting */
		bool password : 1; 			/**< 3.1 password */
		bool username : 1;			/**< 3.1 user name */
	} bits;
} ConnFlags;	/**< connect flags byte */

typedef struct _Packet {
	Header header;
} MqttPacket;

/**
 * connect packet.
 */
typedef struct _ConnectPacket
{
	Header header;	/**< MQTT header byte */
	union
	{
		unsigned char byte;	/**< all connect flags */
		struct
		{
			int : 1;				/**< unused */
			bool cleansess : 1;	/**< cleansession flag */
			bool will : 1;			/**< will flag */
			unsigned int willqos : 2;	/**< will QoS value */
			bool willretain : 1;		/**< will retain setting */
			bool password : 1; 			/**< 3.1 password */
			bool username : 1;			/**< 3.1 user name */
		} bits;
	} flags;	/**< connect flags byte */

	char *protocol, /**< MQTT protocol name */
		*clientid,	/**< string client id */
        *willtopic,	/**< will topic */
        *willmsg,	/**< will payload */
		*username, 
		*password; 

	int keepalive;		/**< keepalive timeout value in seconds */
	unsigned char version;	/**< MQTT version number */
} ConnectPacket;

/**
 * CONNACK packet
 */
typedef struct _ConnAckPacket {
	Header header;
	int rc;
} ConnAckPacket;

/**
 * SUBSCRIBE packet.
 */
typedef struct
{
	Header header;	/**< MQTT header byte */
	int msgid;		/**< MQTT message id */
	list *subtopics;	/**< list of topic strings */
} SubscribePacket;

typedef struct _SubTopic {
	char *topic;
	unsigned char qos;
} SubTopic;

/**
 * SUBACK packet.
 */
typedef struct
{
	Header header;	/**< MQTT header byte */
	int msgid;		/**< MQTT message id */
	list* qoss;		/**< list of granted QoSs */
} SubAckPacket;

/**
 * UNSUBSCRIBE packet.
 */
typedef struct
{
	Header header;	/**< MQTT header byte */
	int msgid;		/**< MQTT message id */
	list* topics;	/**< list of topic strings */
} UnsubscribePacket;

/**
 * PUBLISH packet.
 */
typedef struct
{
	Header header;	/**< MQTT header byte */
	char* topic;	/**< topic string */
	unsigned char qos;
	int msgid;		/**< MQTT message id */
	char* payload;	/**< binary payload, length delimited */
	int payloadlen;
} PublishPacket;

/**
 * one of the ack packets.
 */
typedef struct
{
	Header header;	/**< MQTT header byte */
	int msgid;		/**< MQTT message id */
} AckPacket;

typedef AckPacket PubAckPacket;
typedef AckPacket PubRecPacket;
typedef AckPacket PubRelPacket;
typedef AckPacket PubCompPacket;
typedef AckPacket UnsubackPacket;

int encode_length(char *buf, int length);

void write_header(char **pptr, Header *header);

void write_length(char **ptr, char *bytes, int num);

void write_string(char **pptr, const char *string);

void write_string_len(char **pptr, const char *string, int len);

void write_char(char **pptr, char c);

void write_int(char **pptr, int i);

void write_payload(char **pptr, const char *payload, int length);

char read_char(char** pptr);

int read_int(char** pptr);

char* read_string_len(char **pptr, int *len);

char* read_string(char** pptr);

char* read_string_len(char **pptr, int *len);

#endif /* __MQTT_PACKET_H */

