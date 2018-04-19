


/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.duck.katong;
import com.baidu.mobads.InterstitialAd;
import com.baidu.mobads.InterstitialAdListener;
//import com.baidu.mobads.demo.main.InterstitialAdActivity;
 //import com.baidu.mobads.demo.main.R;


import com.google.ads.*;
import com.google.ads.AdRequest.ErrorCode;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.ViewGroup.LayoutParams;
import android.widget.LinearLayout;
import com.google.ads.AdListener;


public class DuckHuntSuper extends Cocos2dxActivity implements AdListener{
	private static InterstitialAd interAd;
	private static AdView myAdsView;
	static DuckHuntSuper duckContext;
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		
		duckContext=this;
		//setupAds();
		
		//setContentView(R.layout.interstitialad);
		interAd=new InterstitialAd(this);
		interAd.setListener(new InterstitialAdListener(){

			@Override
			public void onAdClick(InterstitialAd arg0) {
				Log.i("InterstitialAd","onAdClick");
			}

			@Override
			public void onAdDismissed() {
				Log.i("InterstitialAd","onAdDismissed");
				interAd.loadAd();
			}

			@Override
			public void onAdFailed(String arg0) {
				Log.i("InterstitialAd","onAdFailed");
				System.out.println("百度广告输出fail");
			}

			@Override
			public void onAdPresent() {
				Log.i("InterstitialAd","onAdPresent");
			}

			@Override
			public void onAdReady() {
				Log.i("InterstitialAd","onAdReady");
				
			}
			
		});
		interAd.loadAd();
		
	  }
   
    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// DuckHuntSuper should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    } 
    

private void setupAds()  
{  
   
    LinearLayout layout = new LinearLayout(this);  
    layout.setOrientation(LinearLayout.VERTICAL);  
    addContentView(layout, new LayoutParams(LayoutParams.FILL_PARENT,LayoutParams.FILL_PARENT));  
    myAdsView = new AdView(this, AdSize.BANNER, "a15379dbdfca227");  
    layout.addView(myAdsView);  
	 myAdsView.loadAd(new AdRequest()); 
	 myAdsView.setVisibility(AdView.GONE);
}  


public void onDismissScreen(Ad arg0) {
	// TODO Auto-generated method stub
	
}

@Override
public void onFailedToReceiveAd(Ad arg0, ErrorCode arg1) {
	// TODO Auto-generated method stub
	
}

@Override
public void onLeaveApplication(Ad arg0) {
	// TODO Auto-generated method stub
	
}

@Override
public void onPresentScreen(Ad arg0) {
	// TODO Auto-generated method stub
	
}

@Override
public void onReceiveAd(Ad arg0) {
	// TODO Auto-generated method stub
	if(interAd!=null){
		//interAd.showAd(duckContext);
	}
}


static MyHandler myHandler = new MyHandler();
static class MyHandler extends Handler {
    public MyHandler(){
        
    }
    public MyHandler(Looper l) {
        super(l);
    }
    
    public void handleMessage(Message msg) {
        super.handleMessage(msg);
        Bundle b = msg.getData();
        if (b.getInt("show") == 1) {
        	myAdsView.setVisibility(AdView.VISIBLE);
        }else if(b.getInt("show") == 0){
        	myAdsView.setVisibility(AdView.GONE);
        }else{
        	
         interAd.showAd(duckContext);
         //interAd.loadAd();
         System.out.println("百度广告输出");
        }
    }
    
}
public static void showMyBannerAds(){
    Message msg = new Message();
    Bundle b = new Bundle();
    b.putInt("show", 1);
    msg.setData(b);
    myHandler.sendMessage(msg);
}
public static void hideMyBannerAds(){
    Message msg = new Message();
    Bundle b = new Bundle();
    b.putInt("show", 0);
    msg.setData(b);
    myHandler.sendMessage(msg);
} 
public static void showMyAds(){
    Message msg = new Message();
    Bundle b = new Bundle();
    b.putInt("show", 3);
    msg.setData(b);
    myHandler.sendMessage(msg);
} 
 
}




