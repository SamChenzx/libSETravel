//
//  gifTestToolsKeyFileMap.h
//  gifTestTools
//
//  Created by Sam Chen on 2021/8/27.
//

#import <Foundation/Foundation.h>
#import "gifTestMockResponse.h"

NS_ASSUME_NONNULL_BEGIN

typedef BOOL(^HTTPStubsTestBlock)(NSURLRequest* request);
typedef gifTestMockResponse* __nonnull (^HTTPStubsResponseBlock)(NSURLRequest* request);

/**
 *  This opaque type represents an installed stub and is used to uniquely
 *  identify a stub once it has been created.
 *
 *  This type is returned by the `stubRequestsPassingTest:withStubResponse:` method
 *  so that you can later reference it and use this reference to remove the stub later.
 *
 *  This type also let you add arbitrary metadata to a stub to differenciate it
 *  more easily when debugging.
 */
@protocol gifTestHTTPStubsDescriptor <NSObject>
/**
 *  An arbitrary name that you can set and get to describe your stub.
 *  Use it as your own convenience.
 *
 *  This is especially useful if you dump all installed stubs using `allStubs`
 *  or if you want to log which stubs are being triggered using `onStubActivation:`.
 */
@property(nonatomic, strong, nullable) NSString* name;
@end

////////////////////////////////////////////////////////////////////////////////
#pragma mark - Interface

/**
 * Stubs Manager. Use this class to add and remove stubs and stub your network requests.
 */
@interface gifTestMockHTTPStubs : NSObject

////////////////////////////////////////////////////////////////////////////////
#pragma mark - Adding & Removing stubs



/**
 *  Dedicated method to add a stub
 *
 *  @param testBlock Block that should return `YES` if the request passed as parameter
 *                   should be stubbed with the response block, and `NO` if it should
 *                   hit the real world (or be managed by another stub).
 *  @param responseBlock Block that will return the `HTTPStubsResponse` (response to
 *                       use for stubbing) corresponding to the given request
 *
 *  @return a stub descriptor that uniquely identifies the stub and can be later used to remove it with `removeStub:`.
 *
 *  @note The returned stub descriptor is retained (`__strong` reference) by `HTTPStubs`
 *        until it is removed (with one of the `removeStub:` / `removeAllStubs`
 *        methods); it is thus recommended to keep it in a `__weak` storage (and not `__strong`)
 *        in your app code, to let the stub descriptor be destroyed and let the variable go
 *        back to `nil` automatically when the stub is removed.
 */
+ (id<gifTestHTTPStubsDescriptor>)stubRequestsPassingTest:(HTTPStubsTestBlock)testBlock
                                         withStubResponse:(HTTPStubsResponseBlock)responseBlock;

/**
 *  Remove a stub from the list of stubs
 *
 *  @param stubDesc The stub descriptor that has been returned when adding the stub
 *                  using `stubRequestsPassingTest:withStubResponse:`
 *
 *  @return `YES` if the stub has been successfully removed, `NO` if the parameter was
 *          not a valid stub identifier
 */
+ (BOOL)removeStub:(id<gifTestHTTPStubsDescriptor>)stubDesc;

/**
 *  Remove all the stubs from the stubs list.
 */
+ (void)removeAllStubs;

////////////////////////////////////////////////////////////////////////////////
#pragma mark - Disabling & Re-Enabling stubs

/**
 *  Enable or disable the stubs for the shared session or for `NSURLConnection`
 *
 *  @param enabled If `YES`, enables the stubs. If `NO`, disable all the
 *                 stubs and let all the requests hit the real world.
 *
 *  @note HTTPStubs are enabled by default, so there is no need to call
 *        this method with `YES` for stubs to work, except if you explicitely
 *        disabled the stubs before.
 *
 *  @note This only affects requests that are further made using `NSURLConnection`
 *        or using `[NSURLSession sharedSession]`. This does not affect requests
 *        sent on an `NSURLSession` created using an `NSURLSessionConfiguration`.
 */
+ (void)setEnabled:(BOOL)enabled;

/**
 *  Whether or not stubs are enabled for the shared session or for `NSURLConnection`
 *
 *  @return If `YES` the stubs are enabled. If `NO` then the stubs are disabled
 */
+ (BOOL)isEnabled;

#if defined(__IPHONE_7_0) || defined(__MAC_10_9)
/**
 *  Enable or disable the stubs on a given `NSURLSessionConfiguration`.
 *
 *  @param enabled If `YES`, enables the stubs for this `NSURLSessionConfiguration`.
 *                 If `NO`, disable the stubs and let all the requests hit the real world
 *  @param sessionConfig The NSURLSessionConfiguration on which to enabled/disable the stubs
 *
 *  @note HTTPStubs are enabled by default on newly created `defaultSessionConfiguration`
 *        and `ephemeralSessionConfiguration`, so there is no need to call this method with
 *        `YES` for stubs to work. You generally only use this if you want to disable
 *        `OHTTPStubs` per `NSURLSession` by calling it before building the `NSURLSession`
 *        with the `NSURLSessionConfiguration`.
 *
 *  @note Important: As usual according to the way `NSURLSessionConfiguration` works, you
 *        MUST set this property BEFORE creating the `NSURLSession`. Once the `NSURLSession`
 *        object is created, they use a deep copy of the `NSURLSessionConfiguration` object
 *        used to create them, so changing the configuration later does not affect already
 *        created sessions.
 */
+ (void)setEnabled:(BOOL)enabled forSessionConfiguration:(NSURLSessionConfiguration *)sessionConfig;

/**
 *  Whether stubs are enabled or disabled on a given `NSURLSessionConfiguration`
 *
 *  @param sessionConfig The NSURLSessionConfiguration on which to enable/disable the stubs
 *
 *  @return If `YES` the stubs are enabled for sessionConfig. If `NO` then the stubs are disabled
 */
+ (BOOL)isEnabledForSessionConfiguration:(NSURLSessionConfiguration *)sessionConfig;
#endif

#pragma mark - Debug Methods

/**
 *  List all the installed stubs
 *
 *  @return An array of `id<gifTestHTTPStubsDescriptor>` objects currently installed. Useful for debug.
 */
+ (NSArray*)allStubs;

/**
 *  Setup a block to be called each time a stub is triggered.
 *
 *  Useful if you want to log all your requests being stubbed for example and see which stub
 *  was used to respond to each request.
 *
 *  @param block The block to call each time a request is being stubbed by OHHTTPStubs.
 *               Set it to `nil` to do nothing. Defaults is `nil`.
 */
+ (void)onStubActivation:( nullable void(^)(NSURLRequest* request, id<gifTestHTTPStubsDescriptor> stub, gifTestMockResponse* responseStub) )block;

/**
 *  Setup a block to be called whenever OHHTTPStubs encounters a redirect request.
 *
 *  @param block The block to call each time a redirect request is being stubbed by OHHTTPStubs.
 *               Set it to `nil` to do nothing. Defaults is `nil`.
 */
+ (void)onStubRedirectResponse:( nullable void(^)(NSURLRequest* request, NSURLRequest* redirectRequest, id<gifTestHTTPStubsDescriptor> stub, gifTestMockResponse* responseStub) )block;

/**
 *  Setup a block to be called each time a stub finishes. Useful if stubs take an insignificant amount
 *  of time to execute (due to low bandwidth or delayed response time). This block may also be called
 *  if there are errors generated by OHHTTPStubs in the course of executing a network request.
 *
 *  @param block The block to call each time a request is finished being stubbed by OHHTTPStubs.
 *               Set it to `nil` to do nothing. Defaults is `nil`.
 */
+ (void)afterStubFinish:( nullable void(^)(NSURLRequest* request, id<gifTestHTTPStubsDescriptor> stub, gifTestMockResponse* responseStub, NSError *error) )block;

/**
 *  Setup a block to be called whenever OHHTTPStubs encounters a missing stub.
 *
 *  @param block The block to call each time no stub for a request can be found.
 *               Set it to `nil` to do nothing. Defaults is `nil`.
 */
+ (void)onStubMissing:( nullable void(^)(NSURLRequest* request) )block;

@end

NS_ASSUME_NONNULL_END

