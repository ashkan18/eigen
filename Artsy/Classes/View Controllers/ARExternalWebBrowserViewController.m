#import "ARExternalWebBrowserViewController.h"
#import <JLRoutes/JLRoutes.h>

@interface TSMiniWebBrowser (Private)
@property(nonatomic, readonly, strong) UIWebView *webView;
@end

@interface ARExternalWebBrowserViewController()<UIGestureRecognizerDelegate>
@property(nonatomic, readonly, strong) UIGestureRecognizer *gesture;
@end

@implementation ARExternalWebBrowserViewController

- (instancetype)initWithURL:(NSURL *)url
{
    self = [super initWithURL:url];
    if (!self) { return nil; }

    self.showNavigationBar = NO;
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    [self.webView constrainTopSpaceToLayoutGuideOfViewController:self predicate:@"0"];
    [self.webView alignTop:nil leading:@"0" bottom:@"0" trailing:@"0" toView:self.view];
    self.scrollView.delegate = self;
    self.scrollView.decelerationRate = UIScrollViewDecelerationRateNormal;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];

    if ([self.navigationController isKindOfClass:[ARNavigationController class]]) {
        UIGestureRecognizer *gesture = self.navigationController.interactivePopGestureRecognizer;

        [ self.scrollView.panGestureRecognizer requireGestureRecognizerToFail:gesture];
        _gesture = gesture;
    }
}

- (void)viewWillDisappear:(BOOL)animated
{
    self.gesture.delegate = nil;
    [super viewWillDisappear:animated];
    [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
}

#pragma mark - Properties

- (UIScrollView *)scrollView
{
    return  self.webView.scrollView;
}

#pragma mark UIScrollViewDelegate

- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    [[ARScrollNavigationChief chief] scrollViewDidScroll:scrollView];
}

#pragma mark UIGestureRecognizerDelegate

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    return YES;
}

#pragma mark UIWebViewDelegate

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    if (navigationType == UIWebViewNavigationTypeLinkClicked) {
        if ([JLRoutes canRouteURL:request.URL]) {
            [JLRoutes routeURL:request.URL];
            return NO;
        }
    }

    return YES;
}

- (BOOL)shouldAutorotate
{
    return [UIDevice isPad];
}

- (NSDictionary *)dictionaryForAnalytics
{
    if (self.currentURL) {
        return @{ @"url" : self.currentURL.absoluteString, @"type" : @"url" };
    }

    return nil;
}

@end
