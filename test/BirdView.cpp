//
// Created by jyp on 2020/11/3.
//

#include  "BirdView/include/BirdView.h"

#include <vector>

int main()
{
    vector<Mat>v(4);

    for (int i = 0; i < 4; i++)
    {
        char buf[100];
        snprintf(buf, sizeof(buf),"/home/jyp/AutoParking/test/%d.jpg", i);
        v[i] = imread(buf);
        printf("%s\n",buf);
    }

    BirdView b;

    //b.setCarSize(240, 380); //
    b.setCarSize(240, 380);
    b.setChessSize(60,60);//
    //b.setMaskHeigth(200);
    //b.setInternalShift(27,27);//
    b.setInternalShift(0, 0);
    //b.sourcePointClick(v);
    b.readConfig();
    //b.getH();
    cout << "test" << endl;
    //while (1)
    //{
    imshow("bird view", b.transformView(v));
    //imwrite("result.jpg",b.transformView(v));
    waitKey(0);
    //}
}