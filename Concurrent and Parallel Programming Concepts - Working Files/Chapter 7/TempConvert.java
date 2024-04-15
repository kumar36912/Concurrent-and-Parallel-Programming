/** Thread safety through 'pure' functions: free of side effects. **/
package tc;

import javax.jws.WebService;
import javax.jws.WebMethod;

/** 'stateless' service consisting of 'pure functions': no fields **/
@WebService
public class TempConvert {
    @WebMethod
    public float c2f(float t) { return 32.0f + (t * 9.0f / 5.0f); }        /** line 1 **/

    @WebMethod
    public float f2c(float t) { return (5.0f / 9.0f) * (t - 32.0f); }      /** line 2 **/
}
