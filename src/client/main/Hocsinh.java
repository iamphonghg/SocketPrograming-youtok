package client.main;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.Scanner;

public class Hocsinh {
    private String MaHS;
    private String TenHS;
    private Integer Namsinh;

    public static void main(String[] args) {
        ArrayList<Hocsinh> lst = new ArrayList<>();
        for (int i = 0; i < 5; i++) {
            System.out.println("Hoc sinh thu " + (i+1));
            lst.add(new Hocsinh());
        }
        lst.sort(new Comparator<Hocsinh>() {
            @Override
            public int compare(Hocsinh o1, Hocsinh o2) {
                return o1.Namsinh - o2.Namsinh;
            }
        });
        for (Hocsinh hocsinh : lst) {
            hocsinh.Hienthi();
        }
    }

    public Hocsinh() {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Ten HS: ");
        String tempTen;
        while (!KiemTraTenHS(tempTen = scanner.nextLine()));
        this.TenHS = tempTen;

        System.out.print("Nam sinh: ");
        int tempNamsinh;
        while (!KiemTraNamsinh(tempNamsinh = scanner.nextInt()));
        this.Namsinh = tempNamsinh;

        System.out.print("Ma HS: ");
        String tempMaHS;
        while (!KiemTraMaHS(tempMaHS = scanner.nextLine()));
        this.MaHS = tempMaHS;
    }

    private boolean KiemTraMaHS(String maHS) {
        if (maHS.length() != 10) return false;
        if (!maHS.substring(0, 2).equals("HS")) return false;
        if (!maHS.substring(2).matches("-?\\d+")) return false;
        return maHS.substring(8).equals(this.Namsinh.toString().substring(2));
    }

    private boolean KiemTraTenHS(String tenHS) {
        if (tenHS.trim().length() != tenHS.length())
            return false;
        String[] splitNames = tenHS.split(" ");
        if (splitNames.length < 2)
            return false;
        for (String splitName : splitNames) {
            if (splitName.trim().length() != splitName.length())
                return false;
            if (Character.isLowerCase(splitName.charAt(0)))
                return false;
            for (int i = 1; i < splitName.length(); i++) {
                if (Character.isUpperCase(splitName.charAt(i)))
                    return false;
            }
        }
        return true;
    }

    private boolean KiemTraNamsinh(int input) {
        return input <= 2015 && input >= 2003;
    }

    public void Hienthi() {
        System.out.println(this.MaHS + " có tên là " + this.TenHS + " sinh năm " + this.Namsinh);
    }
}
