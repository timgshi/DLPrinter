//
//  DLAppDelegate.m
//  DLPrinter
//
//  Created by Timothy Shi on 1/1/14.
//  Copyright (c) 2014 Tim Shi. All rights reserved.
//

#import "DLAppDelegate.h"
#import <CoreLocation/CoreLocation.h>

static NSString * const kParseAppID = @"v6xsQ5BhlyCT5iSybnNIxvEXzqnUXwV3X0HMJrFl";
static NSString * const kParseClientKey = @"SfAmaWipanK5AH8j6QuTZ6HKEcLikjlZgaBJ1cGn";

@interface DLAppDelegate () <CLLocationManagerDelegate>

@property (nonatomic, strong) CLLocationManager *locationManager;

@end

@implementation DLAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [Parse setApplicationId:kParseAppID clientKey:kParseClientKey];
    self.locationManager = [[CLLocationManager alloc] init];
    self.locationManager.delegate = self;
    [self.locationManager startMonitoringSignificantLocationChanges];
    return YES;
}
							
- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations {
    if (locations.count) {
        __block UIBackgroundTaskIdentifier identifier = [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:^{
            [[UIApplication sharedApplication] endBackgroundTask:identifier];
        }];
        PFQuery *query = [PFQuery queryWithClassName:@"DLLocation"];
        [query getFirstObjectInBackgroundWithBlock:^(PFObject *object, NSError *error) {
            if (object) {
                PFGeoPoint *geopoint = [PFGeoPoint geoPointWithLocation:[locations firstObject]];
                [object setObject:geopoint forKey:@"location"];
                [object saveEventually:^(BOOL succeeded, NSError *error) {
                    [[UIApplication sharedApplication] endBackgroundTask:identifier];
                }];
            }
        }];
    }
}

@end
