//
//  SSMUDSocket.h
//  Mudrammer
//
//  Created by Jonathan Hersh on 1/15/13.
//  Copyright (c) 2013 Jonathan Hersh. All rights reserved.
//

#import <GCDAsyncSocket.h>
#import "SSAttributedLineGroup.h"

@protocol SSMUDSocketDelegate;

/**
 * SSMUDSocket encapsulates the network connection for a MUD's session.
 * User-entered commands are passed to the socket for transmission over the network.
 * Text received from the server is parsed into a group of attributed strings
 * for display to the user.
 */
@interface SSMUDSocket : NSObject <GCDAsyncSocketDelegate>

@property (nonatomic, weak) id <SSMUDSocketDelegate> delegate;

- (instancetype) initWithSocket:(GCDAsyncSocket *)socket NS_DESIGNATED_INITIALIZER;

#pragma mark - Connection Lifecycle

// Attempt a connection to the specified hostname and port with a standard timeout.
- (BOOL)connectToHostname:(NSString *)hostname
                   onPort:(NSUInteger)port
                    error:(NSError **)error;

// YES if the socket is neither connected nor in the process of connecting.
- (BOOL)isDisconnected;

// Disconnect immediately.
- (void)disconnect;

// Detaches internal socket delegates.
- (void)resetSocket;

// Send some text (user entered commands/aliases)
- (void)sendUserCommands:(NSArray *)commands;

// NAWS
- (void) sendNAWSWithSize:(CGSize)size;

// Should we echo entered text to the user?
@property (nonatomic, readonly) BOOL shouldEchoText;

@end

// Below protocol methods are called on arbitrary queues
@protocol SSMUDSocketDelegate <NSObject>

@required

// ask the delegate if this connection should attempt to secure itself with SSL
- (BOOL) mudsocketShouldAttemptSSL:(SSMUDSocket *)socket;

// We received text, which was parsed into an attributed line
- (void) mudsocket:(SSMUDSocket *)socket didReceiveAttributedLineGroup:(SSAttributedLineGroup *)group;

@optional

// connecting
- (void) mudsocketDidConnectToHost:(SSMUDSocket *)socket;
- (void) mudsocket:(SSMUDSocket *)socket didDisconnectWithError:(NSError *)err;

// We received some MSSP data for the current world
- (void) mudsocket:(SSMUDSocket *)socket receivedMSSPData:(NSDictionary *)MSSPData;

@end
