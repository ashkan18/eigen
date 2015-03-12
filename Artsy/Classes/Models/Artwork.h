#import "ARPostAttachment.h"
#import "Image.h"
#import "ARHasImageBaseURL.h"
#import "SaleArtwork.h"
#import "ARShareableObject.h"
#import <KSDeferred/KSDeferred.h>
#import "ARHeartStatus.h"

// TODO: Add support ARFollowable for following status

@class Artist, Partner, Profile, Sale, Fair;

typedef NS_ENUM(NSInteger, ARArtworkAvailability) {
    ARArtworkAvailabilityNotForSale,
    ARArtworkAvailabilityForSale,
    ARArtworkAvailabilityOnHold,
    ARArtworkAvailabilitySold
};

typedef NS_ENUM(NSInteger, ARDimensionMetric) {
    ARDimensionMetricInches,
    ARDimensionMetricCentimeters,
    ARDimensionMetricNoMetric
};

@interface Artwork : MTLModel <ARPostAttachment, MTLJSONSerializing, ARHasImageBaseURL, ARShareableObject>

@property (nonatomic, copy) NSString *artworkID;
@property (nonatomic, strong) NSNumber *depth;
@property (nonatomic, strong) NSNumber *diameter;
@property (nonatomic, strong) NSNumber *height;
@property (nonatomic, strong) NSNumber *width;
@property (nonatomic) ARDimensionMetric metric;

@property (nonatomic, copy) NSString *dimensionsCM;
@property (nonatomic, copy) NSString *dimensionsInches;

@property (nonatomic, strong) Artist *artist;
@property (nonatomic, copy) NSString *imageFormatAddress;

@property (nonatomic, strong) Partner *partner;
@property (nonatomic, copy) NSString *collectingInstitution;

// not a property, carried around for fair context
- (Fair *) fair;

// we're just gonna leave these as dictionaries for now
// I think?
@property (nonatomic, copy) NSArray *editionSets;

@property (nonatomic, assign) enum ARArtworkAvailability availability;

@property (nonatomic, copy) NSString *date;
@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *displayTitle;
@property (nonatomic, copy) NSString *exhibitionHistory;
@property (nonatomic, copy) NSString *additionalInfo;
@property (nonatomic, strong) NSNumber *isPriceHidden;
@property (nonatomic, strong, getter = isPublished) NSNumber *published;
@property (nonatomic, copy) NSString *imageRights;
@property (nonatomic, copy) NSString *medium;
@property (nonatomic, copy) NSString *literature;
@property (nonatomic, copy) NSString *provenance;
@property (nonatomic, copy) NSString *series;
@property (nonatomic, copy) NSString *signature;
@property (nonatomic, copy) NSString *category;

@property (nonatomic, copy) NSString *saleMessage;


@property (nonatomic, strong) NSNumber *acquireable;
@property (nonatomic, strong) NSNumber *inquireable;
@property (nonatomic, strong) NSNumber *sold;
@property (nonatomic, strong) NSNumber *forSale;
@property (nonatomic, strong) NSNumber *canShareImage;
@property (nonatomic, strong) NSNumber *auctionResultCount;
@property (nonatomic, strong) Sale *auction;
@property (readonly, nonatomic, assign) BOOL isFollowed;

@property (nonatomic, copy) NSString *price;

@property (nonatomic, copy) NSString *blurb;

@property (nonatomic, strong) NSDate *updatedAt;

@property (nonatomic, strong) Image *defaultImage;

- (ARHeartStatus)heartStatus;

- (AFJSONRequestOperation *)getRelatedArtworks:(void (^)(NSArray *artworks))success;
- (AFJSONRequestOperation *)getRelatedAuctionResults:(void (^)(NSArray *auctionResults))success;
- (AFJSONRequestOperation *)getRelatedFairArtworks:(Fair *)fair success:(void (^)(NSArray *artworks))success;
- (AFJSONRequestOperation *)getRelatedPosts:(void (^)(NSArray *posts))success;
- (AFJSONRequestOperation *)getFeaturedShowsAtFair:(Fair *)fair success:(void (^)(NSArray *shows))success;

/// Gets an update from the server and updates itself, triggers defers from onArtworkUpdate
- (void)updateArtwork;
- (void)updateSaleArtwork;
- (void)updateFair;

/// Adds a callback when the artwork has been update, does not trigger said update.
- (KSPromise *)onArtworkUpdate:(void(^)(void))success failure:(void(^)(NSError *error))failure;
- (KSPromise *)onSaleArtworkUpdate:(void(^)(SaleArtwork *saleArtwork))success failure:(void(^)(NSError *error))failure;
- (KSPromise *)onFairUpdate:(void(^)(Fair *fair))success failure:(void(^)(NSError *error))failure;

- (void)setFollowState:(BOOL)state success:(void (^)(id))success failure:(void (^)(NSError *))failure;
- (void)getFavoriteStatus:(void (^)(ARHeartStatus status))success failure:(void (^)(NSError *error))failure;

- (BOOL)canViewInRoom;
- (BOOL)hasWidth;
- (BOOL)hasHeight;
- (BOOL)hasDepth;
- (BOOL)hasDiameter;
- (BOOL)hasDimensions;
- (BOOL)hasWidthAndHeight;
- (BOOL)hasMoreInfo;
- (BOOL)shouldShowAuctionResults;
- (BOOL)hasMultipleEditions;
- (NSString *)auctionResultsPath;

- (CGFloat)widthInches;
- (CGFloat)heightInches;
- (CGFloat)diameterInches;

- (instancetype)initWithArtworkID:(NSString *)artworkID;

@end
