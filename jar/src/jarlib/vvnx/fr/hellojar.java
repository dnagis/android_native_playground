/*
 * 

adb push ./out/target/product/mido/system/framework/jarvvnx.jar /system/framework

un jar, (i.e. une librairie java), qui peut être lancé par app_process comme dans le script am -> 


app_process -cp /system/framework/jarvvnx.jar /system/bin jarlib.vvnx.fr/hellojar

voir


 */

package jarlib.vvnx.fr;

import android.os.Bundle;
import android.util.Log;

public class hellojar {
    
    private static final String TAG = "jarvvnx";
    
    
    //méthode main(String[]) mandatory sinon AndroidRuntime le lance pas
    public static void main ( String[] args )
		{	  
	  Log.d(TAG, "Hello World from jar Vvnx");
		}
    

}
