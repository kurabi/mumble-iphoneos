/* Copyright (C) 2009-2010 Mikkel Krautz <mikkel@krautz.dk>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#import "Mumble.pb.h"

@class MKConnection;
@class MKPacketDataStream;

typedef enum {
	UDPVoiceCELTAlphaMessage = 0,
	UDPPingMessage,
	UDPVoiceSpeexMessage,
	UDPVoiceCELTBetaMessage
} MKUDPMessageType;

typedef enum {
	VersionMessage = 0,
	UDPTunnelMessage,
	AuthenticateMessage,
	PingMessage,
	RejectMessage,
	ServerSyncMessage,
	ChannelRemoveMessage,
	ChannelStateMessage,
	UserRemoveMessage,
	UserStateMessage,
	BanListMessage,
	TextMessageMessage,
	PermissionDeniedMessage,
	ACLMessage,
	QueryUsersMessage,
	CryptSetupMessage,
	ContextActionAddMessage,
	ContextActionMessage,
	UserListMessage,
	VoiceTargetMessage,
	PermissionQueryMessage,
	CodecVersionMessage
} MKMessageType;

/*
 * MKConnectionDelegate
 */
@protocol MKConnectionDelegate
- (void) connectionOpened:(MKConnection *)conn;
- (void) connectionClosed:(MKConnection *)conn;
- (void) connection:(MKConnection *)conn trustFailureInCertificateChain:(NSArray *)chain;
@end

/*
 * MKMessageHandler
 */
@protocol MKMessageHandler
- (void) handleAuthenticateMessage: (MPAuthenticate *)msg;
- (void) handleBanListMessage: (MPBanList *)msg;
- (void) handleRejectMessage: (MPReject *)msg;
- (void) handleServerSyncMessage: (MPServerSync *)msg;
- (void) handlePermissionDeniedMessage: (MPPermissionDenied *)msg;
- (void) handleUserStateMessage: (MPUserState *)msg;
- (void) handleUserRemoveMessage: (MPUserRemove *)msg;
- (void) handleChannelStateMessage: (MPChannelState *)msg;
- (void) handleChannelRemoveMessage: (MPChannelRemove *)msg;
- (void) handleTextMessageMessage: (MPTextMessage *)msg;
- (void) handleACLMessage: (MPACL *)msg;
- (void) handleQueryUsersMessage: (MPQueryUsers *)msg;
- (void) handlePingMessage: (MPPing *)msg;
- (void) handleCryptSetupMessage: (MPCryptSetup *)msg;
- (void) handleContextActionMessage: (MPContextAction *)msg;
- (void) handleContextActionAddMessage: (MPContextActionAdd *)add;
- (void) handleVersionMessage: (MPVersion *)msg;
- (void) handleUserListMessage: (MPUserList *)msg;
- (void) handleVoiceTargetMessage: (MPVoiceTarget *)msg;
- (void) handlePermissionQueryMessage: (MPPermissionQuery *)msg;
- (void) handleCodecVersionMessage: (MPCodecVersion *)msg;
@end

@interface MKConnection : NSObject {
	@protected
		CFWriteStreamRef writeStream;
		CFReadStreamRef readStream;
		MKMessageType packetType;
		int packetLength;
		int packetBufferOffset;
		NSMutableData *packetBuffer;
		NSString *hostname;
		NSUInteger port;

		BOOL _ignoreSSLVerification;
		id _msgHandler;
		id _delegate;
	
	@public
		BOOL connectionEstablished;
		int socket;
}

- (id) init;
- (void) dealloc;

- (void) connectToHost:(NSString *)hostName port:(NSUInteger)port;
- (void) reconnect;
- (void) closeStreams;
- (BOOL) connected;

- (void) setMessageHandler: (id<MKMessageHandler>)messageHandler;
- (id) messageHandler;
- (void) setDelegate: (id<MKConnectionDelegate>)delegate;
- (id) delegate;

- (void) setIgnoreSSLVerification:(BOOL)flag;

- (void) sendMessageWithType:(MKMessageType)messageType buffer:(unsigned char *)buf length:(NSUInteger)len;
- (void) sendMessageWithType:(MKMessageType)messageType data:(NSData *)data;

- (void) dataReady;
- (void) messageRecieved: (NSData *)data;

- (void) _setupSsl;

- (void) handleError: (CFStreamError)streamError;
- (void) handleSslError: (CFStreamError)streamError;
- (void) handleVoicePacketOfType:(MKUDPMessageType)messageType flags:(NSUInteger)messageFlags datastream:(MKPacketDataStream *)pds;

@end