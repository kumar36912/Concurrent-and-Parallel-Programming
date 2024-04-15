/** 
    String example:
    
    String s1 = new String("HI");
    System.out.println(s1);       // HI
    String s2 = s1.toLowerCase(); // original string is unchanged
    System.out.println(s1);       // HI
    System.out.println(s2);       // hi

    ImmutableRGB API:

    ImmutableRGB sg = new ImmutableRGB(112, 128, 144, "SlateGrey");
    ImmutableRGB invert = sg.invert(); // Creates a new instance.
    int colors = sg.getRGB();          // getter only
    String name = sg.getName();        // getter only

    Clojure (http://clojure.org) relies heavily on immutable structures.
 */

final public class ImmutableRGB {                                            /** line 1 **/
    // Values between 0 and 255.
    final private int red;
    final private int green;
    final private int blue;
    final private String name;

    public ImmutableRGB(int red, int green, int blue, String name) {         /** line 2 **/
        check(red, green, blue);
        this.red = red;
        this.green = green;
        this.blue = blue;
        this.name = name;
    }
    
    // Store three 8-bit values in a 32-bit int.
    public int getRGB() {                                                    /** line 3 **/
        return ((red << 16) | (green << 8) | blue);
    }

    public String getName() {
        return name;
    }

    public ImmutableRGB invert() {                                           /** line 4 **/
        return new ImmutableRGB(255 - red,
                       255 - green,
                       255 - blue,
                       "Inverse of " + name);
    }
    private void check(int red,                                              /** line 5 **/
                       int green,
                       int blue) {
        if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255) 
            throw new IllegalArgumentException();
    }
}
