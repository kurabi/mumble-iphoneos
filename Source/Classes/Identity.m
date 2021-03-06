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

#import "Identity.h"

@implementation Identity

@synthesize primaryKey = _pkey;
@synthesize persistent = _persistent;
@synthesize userName = _userName;
@synthesize fullName = _fullName;
@synthesize emailAddress = _emailAddress;
@synthesize avatarData = _avatarData;

- (id) init {
	self = [super init];
	if (self == nil)
		return nil;

	_pkey = -1;

	return self;
}

- (id) copyWithZone:(NSZone *)zone {
	Identity *ident = [[Identity alloc] init];
	if ([self hasPrimaryKey]) {
		NSLog(@"Identity: Set Primary Key");
		ident.primaryKey = self.primaryKey;
	}
	ident.persistent = self.persistent;
	ident.userName = self.userName;
	ident.fullName = self.fullName;
	ident.emailAddress = self.emailAddress;
	ident.avatarData = self.avatarData;
	return ident;
}

- (BOOL) hasPrimaryKey {
	return _pkey != -1;
}

- (UIImage *) avatar {
	if (_avatarData == nil)
		return nil;

	return [UIImage imageWithData:_avatarData];
}

- (void) setAvatar:(UIImage *)image {
	[_avatarData release];
	if (image == nil) {
		_avatarData = nil;
	} else {
		_avatarData = [UIImagePNGRepresentation(image) retain];
	}
}

@end
