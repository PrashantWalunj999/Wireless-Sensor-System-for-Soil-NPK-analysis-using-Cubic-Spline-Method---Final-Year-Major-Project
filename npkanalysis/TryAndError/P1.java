
class Main {

    class MainInner {
        int a;
        int b;
        int c;

        MainInner(int a,int b,int c) {
            this.a = a;
            this.b = b;
            this.c = c;        
        }
        public void show() {
            System.out.println(a);
            System.out.println(b);
            System.out.println(c);
        }
    }

    public void callMainInner() {
        MainInner obj = new MainInner(10,20,30);
        obj.show();
    }
    public static void main(String[] args) {
        Main obj = new Main();
        obj.callMainInner();
    }
}