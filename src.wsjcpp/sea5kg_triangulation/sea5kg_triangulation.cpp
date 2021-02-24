#include "sea5kg_triangulation.h"
#include <algorithm>

//---------------------------------------------------------------------------
// Sea5kgTriangulationPoint

Sea5kgTriangulationPoint::Sea5kgTriangulationPoint()
: X(0), Y(0), m_nCounter(0) {

};

Sea5kgTriangulationPoint::Sea5kgTriangulationPoint(double X, double Y) 
: X(X), Y(Y), m_nCounter(0) {

};

Sea5kgTriangulationPoint::Sea5kgTriangulationPoint(const Sea5kgTriangulationPoint &p)
: X(p.X), Y(p.Y), m_nCounter(0) {

};

void Sea5kgTriangulationPoint::incCounter() {
    m_nCounter++;
};

int Sea5kgTriangulationPoint::getCounterValue() {
    return m_nCounter;
};

const double &Sea5kgTriangulationPoint::getX() const {
    return X;
}

const double &Sea5kgTriangulationPoint::getY() const {
    return Y;
}

int Sea5kgTriangulationPoint::getXint() const {
    return int(X);
}

int Sea5kgTriangulationPoint::getYint() const {
    return int(Y);
}

void Sea5kgTriangulationPoint::operator+=(const Sea5kgTriangulationPoint &p) {
    X += p.X;
    Y += p.Y;
};

const Sea5kgTriangulationPoint Sea5kgTriangulationPoint::operator / (const int n) {
    Sea5kgTriangulationPoint res_p;

    if (X != 0 && Y != 0) {
        res_p.X += X / n;
        res_p.Y += Y / n;
    }
    return res_p;
};

double Sea5kgTriangulationPoint::calcLength(const Sea5kgTriangulationPoint &p) const {
    double p12_X = (X - p.X);
    double p12_Y = (Y - p.Y);
    return sqrt( p12_X * p12_X + p12_Y * p12_Y);
};

void Sea5kgTriangulationPoint::operator = (const Sea5kgTriangulationPoint &p) {
    X = p.X;
    Y = p.Y;
};

bool Sea5kgTriangulationPoint::operator == (const Sea5kgTriangulationPoint &p) const {
    return (p.X == X) && (p.Y == Y);
};

bool Sea5kgTriangulationPoint::operator != (const Sea5kgTriangulationPoint &p) {
    return (p.X != X) || (p.Y != Y);
};

std::string Sea5kgTriangulationPoint::toString() {
    return "(" + std::to_string(int(X)) + "," + std::to_string(int(Y)) + ")";
};


Sea5kgTriangulationPoint getNextPoint(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2, double length) {
    Sea5kgTriangulationPoint p3(0,0);
    double x1(p1.X), x2(p2.X), y1(p1.Y), y2(p2.Y);
    double del = sqrt( (x2-x1)*(x2-x1) + (y2 - y1)*(y2 - y1) );

    if(del == 0)
        return p3;

    p3.X = x1 + length * (x2-x1) / del;
    p3.Y = y1 + length * (y2-y1) / del;

    return p3;
}

//---------------------------------------------------------------------------
// Sea5kgTriangulationLine

Sea5kgTriangulationLine::Sea5kgTriangulationLine() {

}

Sea5kgTriangulationLine::Sea5kgTriangulationLine(
    Sea5kgTriangulationPoint p1, 
    Sea5kgTriangulationPoint p2
) {
    this->p1 = p1; 
    this->p2 = p2;
};

bool Sea5kgTriangulationLine::hasPoint(const Sea5kgTriangulationPoint &p3)
{
    Sea5kgTriangulationPoint result;
    double len = getPerpendicularToLine(p3, result);
    double x1(p1.X), y1(p1.Y), x2(p2.X), y2(p2.Y);

    return (    len <= 1
        && p3.X <= std::max(x2,x1) && p3.X >= std::min(x2,x1)
        && p3.Y <= std::max(y2,y1) && p3.Y >= std::min(y2,y1)
    );

    return false;
};

//---------------------------------------------------------------------------

double Sea5kgTriangulationLine::length() const  {
    double p12_X = (p1.X - p2.X);
    double p12_Y = (p1.Y - p2.Y);
    return sqrt( p12_X * p12_X + p12_Y * p12_Y);
}

//---------------------------------------------------------------------------

const double Sea5kgTriangulationLine::getPerpendicularToLine(const Sea5kgTriangulationPoint &p3, Sea5kgTriangulationPoint &res)
{
    double x1(p1.X), y1(p1.Y), x2(p2.X), y2(p2.Y), x3(p3.X), y3(p3.Y);

    if(x2 == x1)
    {
        res.X = x1;
        res.Y = p3.Y;
        return Sea5kgTriangulationLine(p3, res).length();
    };

    if(y2 == y1)
    {
        res.X = p3.X;
        res.Y = y1;
        return Sea5kgTriangulationLine(p3, res).length();
    };

    double k = (y2 - y1) / (x2 - x1);
    double b = (x2 * y1 - x1 * y2) / (x2 - x1);
    double k2 = -1 / k;
    double b2 = y3 - k2 * x3;

    res.X = (b2 - b) / (k - k2);
    res.Y = res.X * k2 + b2;

    return Sea5kgTriangulationLine(p3, res).length();
};

bool Sea5kgTriangulationLine::hasIntersection(Sea5kgTriangulationLine &l, Sea5kgTriangulationPoint &result) {
    if(hasIntersection3(l, result))
        return true;
    else if(l.hasIntersection3(*this, result))
        return true;
    return false;
};

Sea5kgTriangulationPoint Sea5kgTriangulationLine::getMiddlePoint() const {
    Sea5kgTriangulationPoint p;
    p.X = (p2.X + p1.X) / 2;
    p.Y = (p2.Y + p1.Y) / 2;
    // p.X = (p1.X < p2.X) ? (p2.X - p1.X) / 2 : (p1.X - p2.X) / 2;
    // p.Y = (p1.Y < p2.Y) ? (p2.Y - p1.Y) / 2 : (p1.Y - p2.Y) / 2;
    return p;
};

bool Sea5kgTriangulationLine::hasIntersection2(Sea5kgTriangulationLine &l, Sea5kgTriangulationPoint &result) {
    // if(getKUngle() == l.getKUngle()) {
    //    return false;
    // }
    // TODO: this function incorrect for vertical lines
    double dx1 = p2.X - p1.X;
    double dy1 = p2.Y - p1.Y;
    double dx2 = l.p2.X - l.p1.X;
    double dy2 = l.p2.Y - l.p1.Y;
    result.X = dy1 * dx2 - dy2 * dx1;
    if(result.X == 0 || dx2 == 0)
        return false;

    result.Y = l.p1.X * l.p2.Y - l.p1.Y * l.p2.X;
    result.X = ((p1.X * p2.Y - p1.Y * p2.X) * dx2 - result.Y * dx1) / result.X;
    result.Y = (dy2 * result.X - result.Y) / dx2;

    return  hasPoint(result) && l.hasPoint(result);
//        (
//            (p1.X <= result.X && p2.X >= result.X)
//            || (p2.X <= result.X && p1.X >= result.X)
//        )
//        &&
//        (
//            (l.p1.X <= result.X && l.p2.X >= result.X)
//            || (l.p2.X <= result.X && l.p1.X >= result.X)
//        );
};


bool Sea5kgTriangulationLine::hasIntersection3(Sea5kgTriangulationLine &l, Sea5kgTriangulationPoint &result) {
    // if(getKUngle() == l.getKUngle()) {
    //     return false;
    // }
    double x1(p1.X), y1(p1.Y), x2(p2.X), y2(p2.Y), x3(l.p1.X), y3(l.p1.Y), x4(l.p2.X), y4(l.p2.Y);

    if (x2 == x1 && x4 == x3) {
        return false;
    } else if (x2 == x1 && x4 != x3) {
        double k = (y4 - y3) / (x4 - x3);
        double b = (x4 * y3 - x3 * y4) / (x4 - x3);
        result.X = x1;
        result.Y = result.X * k + b;
        return  hasPoint(result) && l.hasPoint(result);
    } else if (x2 != x1 && x4 == x3) {
        double k = (y2 - y1) / (x2 - x1);
        double b = (x2 * y1 - x1 * y2) / (x2 - x1);
        result.X = x1;
        result.Y = result.X * k + b;
        return  hasPoint(result) && l.hasPoint(result);
    }

    double dx1 = x2 - x1;
    double dy1 = y2 - y1;
    double dx2 = x4 - x3;
    double dy2 = y4 - y3;
    result.X = dy1 * dx2 - dy2 * dx1;

    if (result.X == 0) {
        return false;
    }

    result.Y = x3 * y4 - y3 * x4;
    result.X = ((x1 * y2 - y1 * x2) * dx2 - result.Y * dx1) / result.X;
    result.Y = (dy2 * result.X - result.Y) / dx2;

    return hasPoint(result) && l.hasPoint(result);
};


//---------------------------------------------------------------------------
// Sea5kgTriangulationTriangle

Sea5kgTriangulationTriangle::Sea5kgTriangulationTriangle() {
    // Nothing
};

//---------------------------------------------------------------------------

Sea5kgTriangulationTriangle::Sea5kgTriangulationTriangle(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2, Sea5kgTriangulationPoint p3)
    : p1(p1), p2(p2), p3(p3)
{

};

//---------------------------------------------------------------------------

double Sea5kgTriangulationTriangle::getSquare() {
    // formule Gerona for square of triangle use half premeter:
    double a = Sea5kgTriangulationLine(p1, p2).length();
    double b = Sea5kgTriangulationLine(p2, p3).length();
    double c = Sea5kgTriangulationLine(p3, p1).length();
    double p = (a + b + c) / 2;
    double res = p * (p - a) * (p - b) * (p - c);
    if(res > 0.1) res = sqrt(res);
    return res;
};

//---------------------------------------------------------------------------

bool Sea5kgTriangulationTriangle::findNearPoint(Sea5kgTriangulationPoint p, Sea5kgTriangulationPoint &res, double r) {
    res = p;
    if (p1.calcLength(p) <= r) {
        res = p1;
    } else if(p2.calcLength(p) <= r) {
        res = p2;
    } else if(p3.calcLength(p) <= r) {
        res = p3;
    }
    return (res != p);
};

//---------------------------------------------------------------------------

/*void Sea5kgTriangulationTriangle::paint(TImage *img) {
    TColor nPenColor = img->Canvas->Pen->Color;
    TColor nBrushColor = img->Canvas->Brush->Color;
    int nWidth = img->Canvas->Pen->Width;

    img->Canvas->Pen->Color = clRed;
    img->Canvas->Pen->Width = 2;
    img->Canvas->Brush->Color = clSilver;

    TPoint points[3];
    points[0] = Point(p1.X,p1.Y);
    points[1] = Point(p2.X,p2.Y);
    points[2] = Point(p3.X,p3.Y);
    img->Canvas->Polygon(points, 2);

    img->Canvas->Pen->Color = nPenColor;
    img->Canvas->Brush->Color = nBrushColor;
    img->Canvas->Pen->Width = nWidth;

    p1.paint(img);
    p2.paint(img);
    p3.paint(img);
};*/

//---------------------------------------------------------------------------

bool Sea5kgTriangulationTriangle::hasIntersection(const Sea5kgTriangulationLine &l) {
    Sea5kgTriangulationPoint p;
    Sea5kgTriangulationLine l0(l.p1,l.p2);
    Sea5kgTriangulationLine l12(p1,p2);
    Sea5kgTriangulationLine l23(p2,p3);
    Sea5kgTriangulationLine l31(p3,p1);

    return
           l12.hasIntersection(l0, p) || l0.hasIntersection(l12, p)
        || l23.hasIntersection(l0, p) || l0.hasIntersection(l23, p)
        || l31.hasIntersection(l0, p) || l0.hasIntersection(l31, p);
};

//---------------------------------------------------------------------------

bool _sort_here_triangle (double x1, double x2) { return (x1 < x2); };

bool Sea5kgTriangulationTriangle::hasPoint(const Sea5kgTriangulationPoint &p) {
    Sea5kgTriangulationArea ar("tmp");
    ar.addPoint(p1.X, p1.Y);
    ar.addPoint(p2.X, p2.Y);
    ar.addPoint(p3.X, p3.Y);
    return ar.hasPoint(p);

    Sea5kgTriangulationLine line_x( Sea5kgTriangulationPoint(-1000, p.Y), Sea5kgTriangulationPoint(1000, p.Y) );
    Sea5kgTriangulationLine line_y( Sea5kgTriangulationPoint(p.X, -1000), Sea5kgTriangulationPoint(p.X,1000) );

    std::vector<double> points_x;
    std::vector<double> points_y;
    points_x.push_back(-1000);
    points_y.push_back(-1000);


    Sea5kgTriangulationLine line12(p1,p2);
    Sea5kgTriangulationLine line23(p2,p3);
    Sea5kgTriangulationLine line31(p3,p2);

    Sea5kgTriangulationPoint p_res;
    if (line12.hasIntersection(line_x, p_res)) {
        points_x.push_back(p_res.X);
    }
    if (line23.hasIntersection(line_x, p_res)) {
        points_x.push_back(p_res.X);
    }
    if(line31.hasIntersection(line_x, p_res)) {
        points_x.push_back(p_res.X);
    }

    if (line12.hasIntersection(line_y, p_res)) {
        points_y.push_back(p_res.Y);
    }
    if (line23.hasIntersection(line_y, p_res)) {
        points_y.push_back(p_res.Y);
    }
    if (line31.hasIntersection(line_y, p_res)) {
        points_y.push_back(p_res.Y);
    }
        

    points_x.push_back(1000);
    points_y.push_back(1000);

    std::sort (points_x.begin(), points_x.end(), _sort_here_triangle);
    std::sort (points_y.begin(), points_y.end(), _sort_here_triangle);

    bool bX = false;
    for(int i = 0; i < points_x.size() -1; i++)
    {
        double x1 = points_x[i];
        double x2 = points_x[i+1];
        if((i+1) % 2 == 0)
        {
            if(x1 <= p.X && p.X <= x2)
                bX = true;
        };
    };

    bool bY = false;
    for(int i = 0; i < points_y.size() -1; i++)
    {
        double y1 = points_y[i];
        double y2 = points_y[i+1];
        if((i+1) % 2 == 0)
        {
            if(y1 <= p.Y && p.Y <= y2)
                bY = true;
        };
    };

    if( bX && bY 
            && line12.getPerpendicularToLine(p, p_res) > 2 
            && line23.getPerpendicularToLine(p, p_res) > 2
            && line31.getPerpendicularToLine(p, p_res) > 2
    )
    {
        return true;
    };
    
    return false; 
};

bool Sea5kgTriangulationTriangle::hasTop(const Sea5kgTriangulationPoint &p) const
{
    double k = 1;
    return ((p1.calcLength(p) < k) || (p2.calcLength(p) < k) || (p3.calcLength(p) < k))
        || p1 == p || p2 == p || p3 == p;
};

bool Sea5kgTriangulationTriangle::isEqual(const Sea5kgTriangulationTriangle &tr)
{
    return hasTop(tr.p1) && hasTop(tr.p2) && hasTop(tr.p3);
};

bool Sea5kgTriangulationTriangle::operator == (const Sea5kgTriangulationTriangle &tr)
{
    return isEqual(tr);
};

std::string Sea5kgTriangulationTriangle::toString() {
    return "{" + p1.toString() + "," + p2.toString() + "," + p3.toString() + "}";
};

// UnicodeString GetPart(UnicodeString str, UnicodeString from, UnicodeString to)
// {
//   str.Delete(1,str.Pos(from)-from.Length() + 1);
//   str = str.SubString(1,str.Pos(to)-1);
//   return str;
// }

// void setPoint(UnicodeString str, Sea5kgTriangulationPoint &p)
// {
//     p.X = StrToInt(str.SubString(1, str.Pos(",")-1));
//     str.Delete(1, str.Pos(","));
//     p.Y = StrToInt(str);
// };

// void triangle::fromString(UnicodeString str)
// {
//     str = GetPart(str, "{", "}");
//     UnicodeString str_p1 = GetPart(str, "(", ")");
//     str.Delete(1, str.Pos(")"));
//     UnicodeString str_p2 = GetPart(str, "(", ")");
//     str.Delete(1, str.Pos(")"));
//     UnicodeString str_p3 = GetPart(str, "(", ")");
// 
//     setPoint(str_p1, p1);
//     setPoint(str_p2, p2);
//     setPoint(str_p3, p3);
// 
//     // return "{ " + p1.toString() + "," + p2.toString() + "," + p3.toString() + "}";
// };

//---------------------------------------------------------------------------

void Sea5kgTriangulationTriangle::operator = (const Sea5kgTriangulationTriangle &tr) {
    p1 = tr.p1;
    p2 = tr.p2;
    p3 = tr.p3;
};

//---------------------------------------------------------------------------
// Sea5kgTriangulationTrianglePointer

Sea5kgTriangulationTrianglePointer::Sea5kgTriangulationTrianglePointer() {
    p1 = new Sea5kgTriangulationPoint();
    p2 = new Sea5kgTriangulationPoint();
    p3 = new Sea5kgTriangulationPoint();
};

Sea5kgTriangulationTriangle Sea5kgTriangulationTrianglePointer::getTriangle() {
  return Sea5kgTriangulationTriangle(get_p1(), get_p2(), get_p3());
};

Sea5kgTriangulationPoint Sea5kgTriangulationTrianglePointer::get_p1() {
  return Sea5kgTriangulationPoint(p1->X, p1->Y);
};

Sea5kgTriangulationPoint Sea5kgTriangulationTrianglePointer::get_p2() {
  return Sea5kgTriangulationPoint(p2->X, p2->Y);
};

Sea5kgTriangulationPoint Sea5kgTriangulationTrianglePointer::get_p3() {
  return Sea5kgTriangulationPoint(p3->X, p3->Y);
};

//---------------------------------------------------------------------------
// Sea5kgTriangulationArea

Sea5kgTriangulationArea::Sea5kgTriangulationArea(std::string sAreaId) {
    m_sAreaId = sAreaId;
}

std::string Sea5kgTriangulationArea::getId() {
    return m_sAreaId;
}

bool _sort_here_area (double x1, double x2) {
    return (x1 < x2);
};

bool Sea5kgTriangulationArea::hasPoint(const Sea5kgTriangulationPoint &p) const {
    double k = 2;
    Sea5kgTriangulationLine line_x( Sea5kgTriangulationPoint(-1000, p.Y), Sea5kgTriangulationPoint(1000, p.Y) );
    Sea5kgTriangulationLine line_y( Sea5kgTriangulationPoint(p.X, -1000), Sea5kgTriangulationPoint(p.X,1000) );

    std::vector<double> points_x;
    std::vector<double> points_y;
    points_x.push_back(-1000);
    points_y.push_back(-1000);

    for (int i = 0; i < m_vPoints.size(); i++) {
        Sea5kgTriangulationPoint p1 = m_vPoints[i];
        Sea5kgTriangulationPoint p2 = m_vPoints[(i+1) % m_vPoints.size()];

        Sea5kgTriangulationLine line_side(p2,p1);

        Sea5kgTriangulationPoint p_res;

        //if(line_x.hasIntersection(line_side, p_res)) // && line_x.hasPoint(p_res) && line_side.hasPoint(p_res) )
        //    points_x.push_back(p_res.X) ;

        if(line_side.hasIntersection(line_x, p_res))
            points_x.push_back(p_res.X);

        //if(line_y.hasIntersection(line_side, p_res)) // && line_y.hasPoint(p_res) && line_side.hasPoint(p_res) )
        //    points_y.push_back(p_res.Y);

        if(line_side.hasIntersection(line_y, p_res)) // && line_y.hasPoint(p_res) && line_side.hasPoint(p_res) )
            points_y.push_back(p_res.Y);


    }

    points_x.push_back(1000);
    points_y.push_back(1000);

    std::sort (points_x.begin(), points_x.end(), _sort_here_area);
    std::sort (points_y.begin(), points_y.end(), _sort_here_area);

    bool bX = false;
    for(int i = 0; i < points_x.size() -1; i++)
    {
        double X_left = points_x[i] - k;
        double X_right = points_x[i+1] + k;

        if( 
            ((i+1) % 2 == 0) 
            && (p.X >= X_left) 
            && (p.X <= X_right)
        )
        {
            bX = true;
            break;
            };
    };

    bool bY = false;
    for(int i = 0; i < points_y.size() -1; i++)
    {
        double Y_left = points_y[i] - k;
        double Y_right = points_y[i+1] + k;

        if(    
            ((i+1) % 2 == 0) 
            && (p.Y >= Y_left)
            && (p.Y <= Y_right)
        )
        {
            bY = true;
            break;
            };
    };

    return bX && bY;
};

bool Sea5kgTriangulationArea::hasLine(const Sea5kgTriangulationLine &L) const {
    if( !hasPoint(L.p1) || !hasPoint(L.p2))
        return false;

    return hasPoint(L.getMiddlePoint());
};

bool Sea5kgTriangulationArea::hasIntersections(Sea5kgTriangulationLine l, std::vector<Sea5kgTriangulationPoint> &result) {
    int nCount = 0;
    for(int i = 0; i < this->count(); i++)
    {
        Sea5kgTriangulationLine line_side(this->getPoint(i),this->getPoint((i+1) % this->count()));

        Sea5kgTriangulationPoint p_res;
        if(l.hasIntersection(line_side, p_res))
        {
            nCount++;
            result.push_back(p_res);
        }
    }
    return (nCount > 0);
};

bool Sea5kgTriangulationArea::hasIntersections(Sea5kgTriangulationArea &ar, std::vector<Sea5kgTriangulationPoint> &result) {
    int nCount = 0;
    for(int ar_i = 0; ar_i < ar.count(); ar_i++) {
        Sea5kgTriangulationPoint ar_p1 = ar.getPoint(ar_i);
        Sea5kgTriangulationPoint ar_p2 = ar.getPoint((ar_i+1) % ar.count());

        Sea5kgTriangulationLine ar_line(ar_p1, ar_p2);


        for (int i = 0; i < this->count(); i++) {
            Sea5kgTriangulationLine line_side(this->getPoint(i),this->getPoint((i+1) % this->count()));
            Sea5kgTriangulationPoint p_res;
            if(line_side.hasIntersection(ar_line, p_res))
            {
                nCount++;
                result.push_back(p_res);
            };
        }
    }
    return (nCount > 0);
};

/*void Sea5kgTriangulationArea::paint(TImage *img)
{
    TColor nColor = img->Canvas->Pen->Color;
    int nWidth = img->Canvas->Pen->Width;

    img->Canvas->Pen->Color = clBlue;
    img->Canvas->Pen->Width = 2;

    if(count() > 0 )
    {
        Sea5kgTriangulationPoint p = getPoint(0);
        img->Canvas->MoveTo(p.X,p.Y);
        img->Canvas->Ellipse(p.X-2,p.Y-2,p.X+2,p.Y+2);
    }

    for(int i = 1; i < count(); i++)
    {
      Sea5kgTriangulationPoint p = getPoint(i);
      img->Canvas->LineTo(p.X,p.Y);
      img->Canvas->Ellipse(p.X-2,p.Y-2,p.X+2,p.Y+2);
    };

    if(count() > 0 )
    {
      Sea5kgTriangulationPoint p = getPoint(0);
      img->Canvas->LineTo(p.X,p.Y);
    };

    img->Canvas->Pen->Color = nColor;
    img->Canvas->Pen->Width = nWidth;

};*/

double Sea5kgTriangulationArea::getSquare() {
    Sea5kgTriangulationPoint pMiddlePoint = getMiddlePoint();
    double square = 0;
    for(int i = 0; i < count(); i++) {
        Sea5kgTriangulationPoint p1 = getPoint(i);
        Sea5kgTriangulationPoint p2 = getPoint((i+1) % count());
        square += Sea5kgTriangulationTriangle(p1, p2, pMiddlePoint).getSquare();
    };
    return square;
};

Sea5kgTriangulationPoint Sea5kgTriangulationArea::getMiddlePoint() {
    Sea5kgTriangulationPoint pMiddlePoint;
    for(int i = 0; i < count(); i++)
        pMiddlePoint += getPoint(i);
    pMiddlePoint = pMiddlePoint / count();
    return pMiddlePoint;
};

const double Sea5kgTriangulationArea::getPerpendicularToLine(
    const Sea5kgTriangulationPoint &p,
    Sea5kgTriangulationPoint &result
) {
    Sea5kgTriangulationPoint res2;
    double nLen2 = 1000;

    for(int i = 0; i < count(); i++)
    {
        Sea5kgTriangulationPoint p1 = getPoint(i);
        Sea5kgTriangulationPoint p2 = getPoint((i+1) % count());
        Sea5kgTriangulationLine L(p1, p2);
        Sea5kgTriangulationPoint res;
        Sea5kgTriangulationPoint p_buff;
        double nLen = L.getPerpendicularToLine(p, res);
        if( nLen < nLen2 && (L.hasPoint(res) || L.getPerpendicularToLine(res, p_buff) < 1))
        {
            res2 = res;
            nLen2 = nLen;
        };
    };

    result = res2;
    return nLen2;
};

bool Sea5kgTriangulationArea::findNearPointSide(const Sea5kgTriangulationPoint &p, Sea5kgTriangulationPoint &result, double len) {
    Sea5kgTriangulationPoint res2;
    double nLen2 = len;

    for (int i = 0; i < count(); i++) {
        Sea5kgTriangulationPoint p1 = getPoint(i);
        double nLen = p1.calcLength(p);
        if(nLen2 >= nLen)
        {
            res2 = p1;
            nLen2 = p1.calcLength(p);
        };
    };

    if (nLen2 < len) {
        result = res2;
        return true;
    };

    nLen2 = len;

    //Sea5kgTriangulationPoint res2;
    //double nLen2 = len;

    for (int i = 0; i < count(); i++) {
        Sea5kgTriangulationPoint p1 = getPoint(i);
        Sea5kgTriangulationPoint p2 = getPoint((i+1) % count());
        Sea5kgTriangulationLine L(p1, p2);
        Sea5kgTriangulationPoint res;
        Sea5kgTriangulationPoint p_buff;
        double nLen = L.getPerpendicularToLine(p, res);
        double nLen_ = L.getPerpendicularToLine(res, p_buff);
        if( nLen < nLen2 && (L.hasPoint(res) && res.calcLength(p_buff) < 1))
        {
            res2 = res;
            nLen2 = nLen;
        };
    };

    if(nLen2 < len)
    {
        result = res2;
        return true;
    };

    return false;
};

bool Sea5kgTriangulationArea::getMinMaxXY(double &minX, double &maxX, double &minY, double &maxY) {
    double _maxX, _minX, _maxY, _minY;

    bool bFirst = true;

    for (int i1 = 0; i1 < count(); i1++) {
        Sea5kgTriangulationPoint p = getPoint(i1);

        if (bFirst) {
            _maxX = p.X;
            _minX = p.X;
            _maxY = p.Y;
            _minY = p.Y;
            bFirst = false;
        };

        if(_maxX < p.X) _maxX = p.X;
        if(_minX > p.X) _minX = p.X;
        if(_maxY < p.Y) _maxY = p.Y;
        if(_minY > p.Y) _minY = p.Y;
    };

    if ((_maxX > _minX) && (_maxY > _minY)) {
       maxX = _maxX;
       minX = _minX;
       maxY = _maxY;
       minY = _minY;
       return true;
    };
    return false;
};


//---------------------------------------------------------------------------
// Sea5kgTriangulationTriangulator

Sea5kgTriangulationTriangulator::Sea5kgTriangulationTriangulator()
: m_nDivisor(2), m_nInfelicity(2.0) {

};

void Sea5kgTriangulationTriangulator::fillArray(const Sea5kgTriangulationPoint &p, std::vector<Sea5kgTriangulationPoint> &p_arr, double r)
{
    Sea5kgTriangulationPoint p_temp;
    Sea5kgTriangulationPoint p_buff;

    std::vector<Sea5kgTriangulationPoint> p_vect;

    p_vect.push_back(p);

    p_temp = p;

    if(findNearPoint(p_temp, p_buff, r)) p_vect.push_back(p_buff);
    else p_vect.push_back(p_temp);
    
    /*p_temp = p_vect[p_vect.size()-1];
    if(findNearPointSide(p_temp, p_buff, r)) p_vect.push_back(p_buff);
    else p_vect.push_back(p_temp);*/

    p_temp = p;
    if(findNearPointSide(p_temp, p_buff, r)) p_vect.push_back(p_buff);
    else p_vect.push_back(p_temp);

    p_temp = p_vect[p_vect.size()-1];
        if(findNearPoint(p_temp, p_buff, r)) p_vect.push_back(p_buff);
    else p_vect.push_back(p_temp);

    p_arr.clear();
    for(unsigned int i = 0; i < p_vect.size(); i++)
    {
        unsigned int id = p_vect.size() - i - 1;
        p_arr.push_back(p_vect[id]);
    };
};

//---------------------------------------------------------------------------

void calcPoints(const Sea5kgTriangulationLine &L, Sea5kgTriangulationPoint &p4, Sea5kgTriangulationPoint &p5, double m_h)
{
    Sea5kgTriangulationPoint p3 = L.getMiddlePoint();

     if(L.p1.X == L.p2.X)
     {
         p4.X = L.p1.X + m_h;
         p4.Y = p3.Y;

         p5.X = L.p1.X - m_h;
         p5.Y = p3.Y;
     }
     else if(L.p1.Y == L.p2.Y )
     {
         p4.Y = L.p1.Y + m_h;
         p4.X = p3.X;

         p5.Y = L.p1.Y - m_h;
         p5.X = p3.X;
     }
     else
     {
         double k1,k2;
         k1 = m_h / L.length();
         k2 = -k1;
         p4.X = p3.X + (L.p1.Y - L.p2.Y) * k1;
         p4.Y = p3.Y - (L.p1.X - L.p2.X) * k1;
         p5.X = p3.X + (L.p1.Y - L.p2.Y) * k2;
         p5.Y = p3.Y - (L.p1.X - L.p2.X) * k2;
     };

};

bool Sea5kgTriangulationTriangulator::addTriangle(
    Sea5kgTriangulationPoint p1,
    Sea5kgTriangulationPoint p2,
    Sea5kgTriangulationPoint p3,
    double a,
    std::vector<Sea5kgTriangulationLine> &m_lines
)
{
    double p = (a + a + a) / 2;
    double min_square = sqrt(p * (p - a) * (p - a) * (p - a));
    min_square = min_square / 10;

    

     Sea5kgTriangulationPoint p_null;

    std::vector<Sea5kgTriangulationPoint> p1_arr;
    std::vector<Sea5kgTriangulationPoint> p2_arr;
    std::vector<Sea5kgTriangulationPoint> p3_arr;

    fillArray(p1, p1_arr, a / m_nDivisor);
    fillArray(p2, p2_arr, a / m_nDivisor);
    fillArray(p3, p3_arr, a / m_nDivisor);

    // possiable triangles 
    std::vector<Sea5kgTriangulationTriangle> triangles;

    for(int i = 0; i < p1_arr.size(); i++ )
    {
        if(p1_arr[i] != p_null && p2_arr[i] != p_null && p3_arr[i] != p_null)
            triangles.push_back(Sea5kgTriangulationTriangle(p1_arr[i], p2_arr[i], p3_arr[i]));
    };

    for(unsigned int i = 0; i < triangles.size(); i++)
    {
        Sea5kgTriangulationTriangle tr = triangles[i];

        if(addTriangleAsIs(tr.p1,tr.p2,tr.p3, a, m_lines))
            return true;
    };

/*    if(addTriangleAsIs(p1,p2,p3, a, m_lines))
        return true;*/


    // return false;
/*
  double k = 0.1;
    Sea5kgTriangulationPoint p_buff;    
    if( findNearPoint(p1, p_buff, a/m_nDivisor) && p_buff.length(p2) > k && p_buff.length(p3) > k && Sea5kgTriangulationTriangle(p_buff,p2,p3).getSquare() >= min_square )    
        p1 = p_buff;

    if( findNearPoint(p2, p_buff, a/m_nDivisor) && p_buff.length(p1) > k && p_buff.length(p3) > k && Sea5kgTriangulationTriangle(p1,p_buff,p3).getSquare() >= min_square )
        p2 = p_buff;
*/    
/*
    bool bDebbug = false;
    if(p1.toString() == "(267,129)" && p2.toString() == "(211,97)")    
    {
        bDebbug = true;
        m_pLogger->info("yes! : " + p3.toString());
    };
 
    if(p1.toString() == "(195,371)" && p2.toString() == "(233,415)")
          m_pLogger->info(" start here!!!!");

      {
        Sea5kgTriangulationPoint p_buff;
        if(m_areas[m_nCurrArea].findNearPointSide(p3, p_buff, a/m_nDivisor))
        {
            if(addTriangleAsIs(p1,p2,p_buff, a, m_lines))
                return true;
        };
    }


    {
        Sea5kgTriangulationPoint p_buff;
        for(unsigned int i2 = 0; i2 < m_areas.size(); i2++)
        {
            if(i2 != m_nCurrArea && m_areas[i2].findNearPointSide(p3, p_buff, a/3))
            {
                if(addTriangleAsIs(p1,p2,p_buff, a, m_lines))
                    return true;
            }
        }
    }


    if(addTriangleAsIs(p1,p2,p3, a, m_lines))
        return true;
*/
    /*
    if(hasCurrentArea(p3))
    {
      // m_pLogger->info("hasCurrentArea == true p3 = " + p3.toString());

        Sea5kgTriangulationPoint p_buff;
        bool br = m_areas[m_nCurrArea].findNearPointSide(p3, p_buff, a/2);
        // m_pLogger->info("hasCurrentArea == true try add(p1,p2,p3): " + Sea5kgTriangulationTriangle(p1,p2,p3).toString());
        

        if(br && Sea5kgTriangulationTriangle(p1,p2,p_buff).getSquare() >= min_square) // && hasCurrentArea(p_buff))
        {
          // m_pLogger->info("hasCurrentArea == true try add: " + Sea5kgTriangulationTriangle(p1,p2,p3).toString());
          // m_pLogger->info("hasCurrentArea == true try add: " + Sea5kgTriangulationTriangle(p1,p2,p_buff).toString());

          // m_pLogger->info("hasCurrentArea not added");
          //p3 = p_buff;
          if(addTriangleAsIs(p1,p2,p_buff, a, m_lines))
            return true;
            };
            // m_pLogger->info("hasCurrentArea == true try add(p1,p2,p_buff): " + Sea5kgTriangulationTriangle(p1,p2,p_buff).toString());

            if(addTriangleAsIs(p1,p2,p3, a, m_lines))
                return true;

     // m_pLogger->info("hasCurrentArea not added");
     }
    else
    {
        // m_pLogger->info("hasCurrentArea == false p3 = " + p3.toString());
        Sea5kgTriangulationPoint p_buff;
        for(unsigned int i2 = 0; i2 < m_areas.size(); i2++)
        {
            if(m_areas[i2].findNearPointSide(p3, p_buff, a/2))
            {
                if(addTriangleAsIs(p1,p2,p_buff, a, m_lines))
                    return true;
            }
        };
    };

    if(bDebbug) m_pLogger->info("1) p3 = " + p3.toString());

    if(addTriangleAsIs(p1,p2,p3, a, m_lines))
        return true;      
     */
    
/*    if(addTriangleAsIs(p1,p2,p3, a, m_lines))
        return true;*/
/*
    if(!hasCurrentArea(p3))
    {    
        double k = 0.1;

        for(unsigned int i2 = 0; i2 < m_areas.size(); i2++ )
        {
            std::vector<Sea5kgTriangulationPoint> result;

            result.clear();
            result.push_back(p3);

            if( m_areas[i2].hasIntersections(Sea5kgTriangulationLine(p1,p3), result) )
            {

                Sea5kgTriangulationPoint p_buff;
                for(int i3 = 0; i3 < result.size(); i3++)
                {
                    bool br = m_areas[i2].findNearPointSide(result[i3], p_buff, a/2);
                    if(bDebbug) m_pLogger->info("2) p3 = " + p_buff.toString());
                    if(addTriangleAsIs(p1,p2,p_buff, a, m_lines))
                        return true;
                };
            };

            result.clear();
            result.push_back(p3);

            if( m_areas[i2].hasIntersections(Sea5kgTriangulationLine(p2,p3), result) )
            {

                Sea5kgTriangulationPoint p_buff;
                for(int i3 = 0; i3 < result.size(); i3++)
                {
                    bool br = m_areas[i2].findNearPointSide(result[i3], p_buff, a/2);

                // if(br && p_buff.length(p1) > k && p_buff.length(p2) > k ) // && hasCurrentArea(p_buff))
                    {
                        if(bDebbug) m_pLogger->info("p3 = " + p_buff.toString());
                        if(addTriangleAsIs(p1,p2,p_buff, a, m_lines))
                            return true;
                    };
                };
            };
        };
    };

*/

    {    
        Sea5kgTriangulationTriangle tr;    
        Sea5kgTriangulationTriangle tr_(p1,p2,p3);    
        Sea5kgTriangulationPoint p_intersection;
        if(hasIntersectionWithOtherTriangles(tr_, tr, p_intersection))
        {
            if(addTriangleAsIs(tr_.p1,tr_.p2,tr.p1, m_nInfelicity, m_lines))
                return true;
        
            if(addTriangleAsIs(tr_.p1,tr_.p2,tr.p2, m_nInfelicity, m_lines))
                return true;
        
            if(addTriangleAsIs(tr_.p1,tr_.p2,tr.p3, m_nInfelicity, m_lines))
                return true;

            //p3 = p_intersection;
            //if(addTriangleAsIs(p1,p2,p3, 1, m_lines))
            //    return true;
        };
    };

    for(int i = 0; i < m_areas[m_nCurrArea].count(); i++)
    {
        Sea5kgTriangulationPoint ar_p1 = m_areas[m_nCurrArea].getPoint(i);
        Sea5kgTriangulationPoint ar_p2 = m_areas[m_nCurrArea].getPoint((i+1) % m_areas[m_nCurrArea].count());
        Sea5kgTriangulationPoint ar_p3 = m_areas[m_nCurrArea].getPoint((i+2) % m_areas[m_nCurrArea].count());
                        
        Sea5kgTriangulationLine L1(ar_p1, ar_p2);
        Sea5kgTriangulationLine L2(ar_p2, ar_p3);
        Sea5kgTriangulationPoint p_buff;
            
        if( (L1.hasPoint(p1) && L2.hasPoint(p2))
            || (L2.hasPoint(p1) && L1.hasPoint(p2))
            || (ar_p1.calcLength(p1) < m_nInfelicity || ar_p2.calcLength(p2) < m_nInfelicity)
            || (ar_p1.calcLength(p2) < m_nInfelicity || ar_p2.calcLength(p1) < m_nInfelicity)
        )
        {            
            if(Sea5kgTriangulationLine(p1,ar_p2).length() < a + a/m_nDivisor && Sea5kgTriangulationLine(p2,ar_p2).length() < a + a/m_nDivisor)
            {
                  if(addTriangleAsIs(p1,p2,ar_p2, m_nInfelicity, m_lines))
                    return true;
            };
        };
    };

    return false;
};

//---------------------------------------------------------------------------

bool Sea5kgTriangulationTriangulator::addTriangleAsIs(
    Sea5kgTriangulationPoint p1, 
    Sea5kgTriangulationPoint p2, 
    Sea5kgTriangulationPoint p3, 
    double a,
    std::vector<Sea5kgTriangulationLine> &m_lines
    )
{
    double p = (a + a + a) / 2;
    double min_square = sqrt(p * (p - a) * (p - a) * (p - a));
    min_square = min_square / 3;

    double r = a/m_nDivisor;
    Sea5kgTriangulationPoint p_altr;    
    if(findNearPoint(p1, p_altr, r))  p1 = p_altr;
    if(findNearPoint(p2, p_altr, r))  p2 = p_altr;
    if(findNearPoint(p3, p_altr, r))  p3 = p_altr;


//    for(unsigned int i = 0; i < triangles.size(); i++)
//    {
        Sea5kgTriangulationTriangle tr(p1,p2,p3);

        if( !hasCurrentArea(tr.p1) || !hasCurrentArea(tr.p2) || !hasCurrentArea(tr.p3) )
            return false;

        if( !hasCurrentArea(tr.p1,tr.p2) || !hasCurrentArea(tr.p2,tr.p3) || !hasCurrentArea(tr.p3,tr.p1) )
            return false;

        Sea5kgTriangulationTriangle tr_buff;
        Sea5kgTriangulationPoint p_intersection;                                                                 

        if(tr.p1.calcLength(tr.p2) < m_nInfelicity || tr.p1.calcLength(tr.p3) < m_nInfelicity || tr.p2.calcLength(tr.p3) < m_nInfelicity )
            return false;

        if(
                !findTriangle(tr.p1,tr.p2,tr.p3) 
                && !hasIntersectionWithOtherTriangles(tr, tr_buff, p_intersection)
        )
        {        
            if(tr.getSquare() > min_square)        
            {
                m_triangles.push_back(tr);
              m_lines.push_back(Sea5kgTriangulationLine(tr.p1,tr.p3));
              m_lines.push_back(Sea5kgTriangulationLine(tr.p2,tr.p3));

              // m_pLogger->info(Sea5kgTriangulationTriangle(p1,p2,p3).toString());
              //if(tr.p2.toString() == "(195,371)" && tr.p3.toString() == "(233,415)")
              //   m_pLogger->info(" add line here!!!!");

              //m_lines.push_back(Sea5kgTriangulationLine(tr.p2,tr.p3));
              return true;
            };
        }
//      };
    return false;
};

//---------------------------------------------------------------------------

bool Sea5kgTriangulationTriangulator::hasCurrentArea(Sea5kgTriangulationPoint p)
{
    for(int i = 0; i < m_areas.size(); i++)
    {
        if( i != m_nCurrArea && m_areas[i].hasPoint(p) )
        {
            Sea5kgTriangulationPoint p_buff;
            if(m_areas[i].getPerpendicularToLine(p, p_buff) > m_nInfelicity)
                return false;
        };
    }
        
    return m_areas[m_nCurrArea].hasPoint(p);
};

//---------------------------------------------------------------------------

bool Sea5kgTriangulationTriangulator::hasCurrentArea(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2)
{
    Sea5kgTriangulationLine L(p1,p2);
    for(int i = 0; i < m_areas.size(); i++)
    {
        if( i != m_nCurrArea && m_areas[i].hasLine(L) )
            return false;
    }
    return m_areas[m_nCurrArea].hasLine(L);
};

//---------------------------------------------------------------------------

bool hasPoint_(Sea5kgTriangulationTriangle tr, const Sea5kgTriangulationPoint &p, Sea5kgTriangulationTriangle &tr_result, Sea5kgTriangulationPoint &p_result, Sea5kgTriangulationTriangle tr1)
{
    if( (tr.hasPoint(p) && !tr.hasTop(p)))
    {
        p_result = p;
        tr_result = tr1;
        return true;            
    }    
    return false;
};


bool Sea5kgTriangulationTriangulator::hasIntersectionWithOtherTriangles(const Sea5kgTriangulationTriangle &tr_, Sea5kgTriangulationTriangle &result, Sea5kgTriangulationPoint &p_result)
{
    Sea5kgTriangulationLine L[3];
    L[0] = Sea5kgTriangulationLine(tr_.p1,tr_.p2);
    L[1] = Sea5kgTriangulationLine(tr_.p2,tr_.p3);
    L[2] = Sea5kgTriangulationLine(tr_.p3,tr_.p1);

    for(int i = 0; i < m_triangles.size(); i++)
    {
        Sea5kgTriangulationTriangle tr = m_triangles[i];
        Sea5kgTriangulationLine TR_L[3];

/*        if( hasPoint_(tr, p1, result, p_result, tr) )
            return true;    

        if( hasPoint_(tr, p2, result, p_result, tr) )
            return true;    

        if( hasPoint_(tr, p3, result, p_result, tr) )
            return true;    

        if( hasPoint_(tr_, tr.p1, result, p_result, tr) )
            return true;    

        if( hasPoint_(tr_, tr.p2, result, p_result, tr) )
            return true;    

        if( hasPoint_(tr_, tr.p3, result, p_result, tr) )
            return true;    
*/
        TR_L[0] = Sea5kgTriangulationLine(m_triangles[i].p1, m_triangles[i].p2);
        TR_L[1] = Sea5kgTriangulationLine(m_triangles[i].p2, m_triangles[i].p3);
        TR_L[2] = Sea5kgTriangulationLine(m_triangles[i].p3, m_triangles[i].p1);

/*
        if( hasPoint_(tr, L[0].getMiddlePoint(), result, p_result, tr) )
            return true;    

        if( hasPoint_(tr, L[1].getMiddlePoint(), result, p_result, tr) )
            return true;    

        if( hasPoint_(tr, L[2].getMiddlePoint(), result, p_result, tr) )
            return true;    
*/

/*
        if( hasPoint_(tr_, TR_L[0].getMiddlePoint(), result, p_result, tr) )
            return true;    

        if( hasPoint_(tr_, TR_L[1].getMiddlePoint(), result, p_result, tr) )
            return true;    

        if( hasPoint_(tr_, TR_L[2].getMiddlePoint(), result, p_result, tr) )
            return true;    
*/
        Sea5kgTriangulationPoint p;

        for (int i1 = 0; i1 < 3; i1++)
            for (int i2 = 0; i2 < 3; i2++)
                if( TR_L[i1].hasIntersection(L[i2], p) && TR_L[i1].hasPoint(p) && L[i2].hasPoint(p) && !tr.hasTop(p) && !tr_.hasTop(p))
                {
                    p_result = p;
                    result = tr;
                    return true;
                };

      /*if(tr.hasPoint(p1) || tr.hasPoint(p2) || tr.hasPoint(p3))
      {
            result = tr;
            p_result = Sea5kgTriangulationPoint();
            return true;      
      };*/

    };
    return false;
};
//---------------------------------------------------------------------------

void Sea5kgTriangulationTriangulator::addArea(Sea5kgTriangulationArea ar)
{
    std::vector<Sea5kgTriangulationPoint> result;
    
    for(int i = 0; i < m_areas.size(); i++)
    {
        if(m_areas[i].hasIntersections(ar, result))
        {
            // ShowMessage("areas has intersection");
            return;
        };
    };
        
    m_areas.push_back(ar);
};
//---------------------------------------------------------------------------

std::vector<Sea5kgTriangulationArea> &Sea5kgTriangulationTriangulator::getAreas()
{
    return m_areas;
};
//---------------------------------------------------------------------------

std::vector<Sea5kgTriangulationTriangle> &Sea5kgTriangulationTriangulator::getTriangles()
{
    return m_triangles;
};
//---------------------------------------------------------------------------

void Sea5kgTriangulationTriangulator::findPoints(
    const Sea5kgTriangulationPoint &p1, 
    const std::vector<Sea5kgTriangulationPoint> &points, 
    double a, 
    std::vector<Sea5kgTriangulationTriangle> &triangles
)
{
    double k = 2;
    double a_min2 = k;
    double a_min1 = a - k;
    double a_max = a + a/m_nDivisor + k;
    for(unsigned int iP2 = 0; iP2 < points.size(); iP2++)
    for(unsigned int iP3 = 0; iP3 < points.size(); iP3++)
    {
        if(iP2 == iP3) continue;

        Sea5kgTriangulationPoint p2 = points[iP2];
        Sea5kgTriangulationPoint p3 = points[iP3];
        
        double nLen1 = p1.calcLength(p2);
        double nLen2 = p2.calcLength(p3);
        double nLen3 = p3.calcLength(p1);

        
        if(
            nLen1 > a_min2 &&  nLen2 > a_min2 && nLen3 > a_min2 &&
            nLen1 <= a_max && nLen2 <= a_max && nLen3 <= a_max
        )
        {
            triangles.push_back(Sea5kgTriangulationTriangle(p1,p2,p3));
        };
    
        //if(nLen1 <= (a + k))
        //    triangles.push_back(Sea5kgTriangulationTriangle(p1,p2,p3));
    };
/*    
    for(unsigned int iP2 = 0; iP2 < points.size(); iP2++)
    for(unsigned int iP3 = 0; iP3 < points.size(); iP3++)
    {
        if(iP2 == iP3) continue;

        Sea5kgTriangulationPoint p2 = points[iP2];
        Sea5kgTriangulationPoint p3 = points[iP3];
        
        double nLen1 = p1.length(p2);
        double nLen2 = p2.length(p3);
        double nLen3 = p3.length(p1);

        
        if(
            nLen1 > a_min2 &&  nLen2 > a_min2 && nLen3 > a_min2 &&
            nLen1 <= a_min1 && nLen2 <= a_min1 && nLen3 <= a_min1
        )
        {
            triangles.push_back(Sea5kgTriangulationTriangle(p1,p2,p3));
        };
    
        //if(nLen1 <= (a + k))
        //    triangles.push_back(Sea5kgTriangulationTriangle(p1,p2,p3));
    };
*/
    if( triangles.size() == 0 && points.size() > 0)
    {
        findPoints(p1, points, a + a/m_nDivisor, triangles);
    };
};

void Sea5kgTriangulationTriangulator::triangulate_add_triangles() {

};

void Sea5kgTriangulationTriangulator::triangulate_resizing() {

};

void Sea5kgTriangulationTriangulator::triangulate_remove_triangles() {

};

void Sea5kgTriangulationTriangulator::step_first() {
    for(int i = 0; i < m_areas.size(); i++)
    {
        m_nCurrArea = i;

        int nCountTriangles = m_areas[m_nCurrArea].getCountTriangles();
        double square = m_areas[m_nCurrArea].getSquare();
        double approximateTriangle = square / m_areas[m_nCurrArea].getCountTriangles();
        double a = sqrt((approximateTriangle * 4.0)/ sqrt(3));
        double h = 2 * approximateTriangle / a;
        double r = a * a * a / sqrt((3*a) * a * a * a );

        // m_pLogger->info("All square: " + FloatToStr(square) + " px^2");
        // m_pLogger->info("Approximate area of one triangle: " + FloatToStr(approximateTriangle) + " px^2");
        // m_pLogger->info("a = " + FloatToStr(a) + " px");
        // m_pLogger->info("h = " + FloatToStr(h) + " px");
        // m_pLogger->info("r = " + FloatToStr(r) + " px");

        
        std::vector<Sea5kgTriangulationPoint> points;
        double nStartX = 0;
         Sea5kgTriangulationPoint p_before;
        for(double iY = 0; iY < 1024; iY = iY + h)
        {
            if( nStartX < 0 )
              nStartX = 0;
            else
              nStartX = (-a)/2;

            p_before = Sea5kgTriangulationPoint();

            for(double iX = nStartX; iX < 1024; iX = iX + a)
            {
                Sea5kgTriangulationPoint p(iX, iY);
                Sea5kgTriangulationPoint p_buff;


                if(hasCurrentArea(p))
                {
                    //points.push_back(p);

                    if(findNearPointSide(p, p_buff, a/m_nDivisor))
                        points.push_back(p_buff);
                    else
                        points.push_back(p);
                }
                else if(findNearPointSide(p, p_buff, a/m_nDivisor))
                {                
                    //m_pLogger->info("added p = " + p5.toString());
                    points.push_back(p_buff);    
                }


/*
                Sea5kgTriangulationLine same_line[5];
                Sea5kgTriangulationPoint p4, p5;
                same_line[0] = Sea5kgTriangulationLine(p, p_before);
                                calcPoints(same_line[0], p4, p5, h);
                                same_line[1] = Sea5kgTriangulationLine(p, p4);
                                same_line[2] = Sea5kgTriangulationLine(p_before, p4);
                                same_line[3] = Sea5kgTriangulationLine(p, p5);
                                same_line[4] = Sea5kgTriangulationLine(p_before, p5);
                                

                for(int iL = 0; iL < 5; iL++)
                {
                    std::vector<Sea5kgTriangulationPoint> points_intersection;
                    if( m_areas[m_nCurrArea].hasIntersections(same_line[iL], points_intersection) )
                    {
                        m_pLogger->info("p4 = " + p4.toString());
                        m_pLogger->info("p5 = " + p5.toString());
                        points.push_back(points_intersection[0]);
                    };
                };
*/
/*                for(unsigned int iPs = 0; iPs < points_intersection.size(); iPs++)
                {
                    points.push_back(points_intersection[iPs]);

//                    if(hasCurrentArea(p_buff))
                    //points.push_back(p_buff);
                    // points.push_back(p_buff);
                };*/
                p_before = p;
            };
        };
        
        for(unsigned int iP = 0; iP < points.size(); iP++)
        {
            std::vector<Sea5kgTriangulationTriangle> triangles;
            std::vector<Sea5kgTriangulationLine> lines;
            Sea5kgTriangulationPoint p1 = points[iP];
            findPoints(p1, points, a, triangles);
            int sch = 0;
            for(unsigned int iT = 0; iT < triangles.size(); iT++)
            {
                Sea5kgTriangulationTriangle tr = triangles[iT];
                Sea5kgTriangulationTriangle tr_;
                Sea5kgTriangulationPoint p_intersection;

                if(addTriangleAsIs(tr.p1,tr.p2,tr.p3, a, lines))
                {
                    sch++;
                }

                               /*if(!findTriangle(tr.p1,tr.p2,tr.p3) && !hasIntersectionWithOtherTriangles(tr, tr_, p_intersection))
                   m_triangles.push_back(tr);*/
            };
//            if(triangles.size() != 0 && triangles.size() - sch == )
        };
    };
};

bool findNearPoint_sp(std::vector<Sea5kgTriangulationPoint *> &pPoints, Sea5kgTriangulationPoint *p, Sea5kgTriangulationPoint **pResult, double a)
{
    Sea5kgTriangulationPoint pP(p->X, p->Y);
    double curr_len = -1;    
    int iFound = -1;
    for(unsigned int i = 0; i < pPoints.size(); i++)
    {
        double len = pPoints[i]->calcLength(pP);

        if(len < a)
        {
            if(iFound == -1)
            {
                curr_len = len;
                iFound = i; 
            }
            else if(curr_len > len)
            {
                curr_len = len;
                iFound = i;
            }
        }    
    };

    if(iFound == -1) return false;

    *pResult = pPoints[iFound];
    return true;
};

void Sea5kgTriangulationTriangulator::step_first_alien()
{
    for(int i = 0; i < m_areas.size(); i++)
    {
        m_nCurrArea = i;

        int nCountTriangles = m_areas[m_nCurrArea].getCountTriangles();
        double square = m_areas[m_nCurrArea].getSquare();
        double approximateTriangle = square / m_areas[m_nCurrArea].getCountTriangles();
        double a = sqrt((approximateTriangle * 4.0)/ sqrt(3));
        double h = 2 * approximateTriangle / a;
        double r = a * a * a / sqrt((3*a) * a * a * a );

        // m_pLogger->info("All square: " + FloatToStr(square) + " px^2");
        // m_pLogger->info("Approximate area of one triangle: " + FloatToStr(approximateTriangle) + " px^2");
        // m_pLogger->info("a = " + FloatToStr(a) + " px");
        // m_pLogger->info("h = " + FloatToStr(h) + " px");
        // m_pLogger->info("r = " + FloatToStr(r) + " px");
        
        std::vector<Sea5kgTriangulationPoint *> pPoints;
        std::vector<Sea5kgTriangulationTrianglePointer *> pTriangules;
        double nStartX = 0, nEndX = 0;
        double minX,maxX,minY,maxY;

        m_areas[m_nCurrArea].getMinMaxXY(minX, maxX, minY, maxY);

        // m_pLogger->info("minX = " + FloatToStr(minX) + " px");
        // m_pLogger->info("maxX = " + FloatToStr(maxX) + " px");
        // m_pLogger->info("minY = " + FloatToStr(minY) + " px");
        // m_pLogger->info("maxY = " + FloatToStr(maxY) + " px");

        double a2 = a/2;
        double a4 = a/4;
        double a10 = a/10;
        minX -= a;
        maxX += a;

        int iS = 0;
        for(double iY = minY - h; iY < maxY + h; iY = iY + h)
        {
            iS = iS + 1;
            if( iS % 2 == 0)
            {        
                nStartX = minX;
                nEndX = maxX;
            }
            else
            {
                nStartX = minX - a2;
                nEndX = maxX + a2;
            }

            for(double iX = nStartX; iX < nEndX; iX = iX + a)
            {
                  Sea5kgTriangulationPoint p1_(iX, iY);
                Sea5kgTriangulationPoint p2_(iX + a, iY);
                Sea5kgTriangulationPoint p3_;
                Sea5kgTriangulationPoint p4_;
                Sea5kgTriangulationLine li_(p1_,p2_);

                Sea5kgTriangulationPoint *p1 = new Sea5kgTriangulationPoint(p1_.X, p1_.Y);
                Sea5kgTriangulationPoint *p2 = new Sea5kgTriangulationPoint(p2_.X, p2_.Y);
                
                calcPoints(li_,p3_,p4_,h);
                
                Sea5kgTriangulationPoint *p3 = new Sea5kgTriangulationPoint(p3_.X, p3_.Y);
                Sea5kgTriangulationPoint *p4 = new Sea5kgTriangulationPoint(p4_.X, p4_.Y);

                Sea5kgTriangulationPoint *pFound;

                /*
                pPoints.push_back(p1);
                pPoints.push_back(p2);
                pPoints.push_back(p3);
                pPoints.push_back(p4);
                */
                // m_pLogger->info(" find Near Point ");

                if( findNearPoint_sp(pPoints, p1, &pFound, a4))
                    *p1 = *pFound;
                else
                    pPoints.push_back(p1);

                if( findNearPoint_sp(pPoints, p2, &pFound, a4))
                    *p2 = *pFound;
                else
                    pPoints.push_back(p2);

                if( findNearPoint_sp(pPoints, p3, &pFound, a4))
                    *p3 = *pFound;
                else
                    pPoints.push_back(p3);

                if( findNearPoint_sp(pPoints, p4, &pFound, a4))
                    *p4 = *pFound;
                else
                    pPoints.push_back(p4);

                // m_pLogger->info(" create triangle ");

                Sea5kgTriangulationTrianglePointer *pTrinagle1 = new Sea5kgTriangulationTrianglePointer();
                *(pTrinagle1->p1) = *p1;
                *(pTrinagle1->p2) = *p2;
                *(pTrinagle1->p3) = *p3;

                Sea5kgTriangulationTrianglePointer *pTrinagle2 = new Sea5kgTriangulationTrianglePointer();
                *(pTrinagle2->p1) = *p1;
                *(pTrinagle2->p2) = *p2;
                *(pTrinagle2->p3) = *p4;

                // m_pLogger->info(" push ");

                pTriangules.push_back(pTrinagle1);
                pTriangules.push_back(pTrinagle2);                    
            }
        }

        // m_pLogger->info(" remove triangle which not inside in current area ");


        // remove triangle which has not inside in current area
        std::vector<Sea5kgTriangulationTrianglePointer *> pTriangules_copy = pTriangules;
        pTriangules.clear();
        for(int iT = 0; iT < pTriangules_copy.size(); iT++)
        {
            Sea5kgTriangulationTriangle tr = pTriangules_copy[iT]->getTriangle();

            if(hasCurrentArea(tr.p1) || hasCurrentArea(tr.p2) || hasCurrentArea(tr.p3))
                pTriangules.push_back(pTriangules_copy[iT]);
        }


        // m_pLogger->info(" find point near with area of side ");

        // find point near with area of side 
        for(int iT = 0; iT < pTriangules.size(); iT++)
        {
            Sea5kgTriangulationTrianglePointer *tr = pTriangules[iT];
            Sea5kgTriangulationPoint p1(tr->p1->X, tr->p1->Y);
            Sea5kgTriangulationPoint p2(tr->p2->X, tr->p2->Y);
            Sea5kgTriangulationPoint p3(tr->p3->X, tr->p3->Y);

            Sea5kgTriangulationPoint p_buff;

            if(findNearPointSide(p1, p_buff, a2))
            {
                tr->p1->X = p_buff.X;
                tr->p1->Y = p_buff.Y;
            }

            if(findNearPointSide(p2, p_buff, a2))
            {
                tr->p2->X = p_buff.X;
                tr->p2->Y = p_buff.Y;
            }

            if(findNearPointSide(p3, p_buff, a2))
            {
                tr->p3->X = p_buff.X;
                tr->p3->Y = p_buff.Y;
            }
        };
        
        // remove triangles which has point in inside current area
        pTriangules_copy.clear();
        pTriangules_copy = pTriangules;
        pTriangules.clear();
        for(int iT = 0; iT < pTriangules_copy.size(); iT++)
        {
            Sea5kgTriangulationTriangle tr = pTriangules_copy[iT]->getTriangle();
            Sea5kgTriangulationPoint p12 = Sea5kgTriangulationLine(tr.p1, tr.p2).getMiddlePoint();
            Sea5kgTriangulationPoint p23 = Sea5kgTriangulationLine(tr.p2, tr.p3).getMiddlePoint();
            Sea5kgTriangulationPoint p31 = Sea5kgTriangulationLine(tr.p3, tr.p1).getMiddlePoint();

            if( hasCurrentArea(tr.p1) && 
                hasCurrentArea(tr.p2) && 
                hasCurrentArea(tr.p3)
            )
            {
                if( !hasCurrentArea(p12) )
                {
                    Sea5kgTriangulationTrianglePointer *tr_ = new Sea5kgTriangulationTrianglePointer();

                    Sea5kgTriangulationPoint *new_p = new Sea5kgTriangulationPoint();
                    Sea5kgTriangulationPoint p_buff;

                    if(findNearPointSide(p12, p_buff, a2))
                        p12 = p_buff;

                    //Sea5kgTriangulationPoint p1_12 = Sea5kgTriangulationLine(tr.p1, p1_12).getMiddlePoint();
                    //Sea5kgTriangulationPoint p2_12 = Sea5kgTriangulationLine(tr.p2, p2_12).getMiddlePoint();

                    if( hasCurrentArea(p12) ) // && hasCurrentArea(p1_12) && hasCurrentArea(p2_12))
                    {
                        new_p->X = p12.X;
                        new_p->Y = p12.Y;

                        tr_->p1 = new_p;
                        *(tr_->p2) = *(pTriangules_copy[iT]->p2);
                        *(tr_->p3) = *(pTriangules_copy[iT]->p3);
                                        
                        *(pTriangules_copy[iT]->p2) = *(tr_->p1);

                        pTriangules.push_back(tr_);
                    }
                }                    

                if( !hasCurrentArea(p23) ) {
                    Sea5kgTriangulationTrianglePointer *tr_ = new Sea5kgTriangulationTrianglePointer();

                    Sea5kgTriangulationPoint *new_p = new Sea5kgTriangulationPoint();
                    Sea5kgTriangulationPoint p_buff;

                    if(findNearPointSide(p23, p_buff, a2))
                        p23 = p_buff;
                    
                    // Sea5kgTriangulationPoint p2_23 = Sea5kgTriangulationLine(tr.p2, p2_23).getMiddlePoint();
                    // Sea5kgTriangulationPoint p3_23 = Sea5kgTriangulationLine(tr.p3, p3_23).getMiddlePoint();

                    if( hasCurrentArea(p23) ) // && hasCurrentArea(p2_23) && hasCurrentArea(p3_23))
                    {

                        new_p->X = p23.X;
                        new_p->Y = p23.Y;

                        *(tr_->p1) = *(pTriangules_copy[iT]->p1);
                        tr_->p2 = new_p;
                        *(tr_->p3) = *(pTriangules_copy[iT]->p3);
                                    
                        *(pTriangules_copy[iT]->p3) = *(tr_->p2);

                        pTriangules.push_back(tr_);
                    }
                }                    

                if( !hasCurrentArea(p31) )
                {
                    Sea5kgTriangulationTrianglePointer *tr_ = new Sea5kgTriangulationTrianglePointer();

                    Sea5kgTriangulationPoint *new_p = new Sea5kgTriangulationPoint();
                    Sea5kgTriangulationPoint p_buff;

                    if(findNearPointSide(p31, p_buff, a2))
                        p31 = p_buff;

                    // Sea5kgTriangulationPoint p3_31 = Sea5kgTriangulationLine(tr.p3, p3_31).getMiddlePoint();
                    // Sea5kgTriangulationPoint p1_31 = Sea5kgTriangulationLine(tr.p1, p1_31).getMiddlePoint();
                                            
                    if( hasCurrentArea(p31) ) // && hasCurrentArea(p3_31) && hasCurrentArea(p1_31))
                    {
                        new_p->X = p31.X;
                        new_p->Y = p31.Y;

                        *(tr_->p1) = *(pTriangules_copy[iT]->p1);
                        *(tr_->p2) = *(pTriangules_copy[iT]->p2);
                        tr_->p3 = new_p;
                                            
                        *(pTriangules_copy[iT]->p1) = *(tr_->p3);

                        pTriangules.push_back(tr_);    
                    }
                }                    



                pTriangules.push_back(pTriangules_copy[iT]);
                
            }
            
        }
        
        // remove triangles which as line        
        pTriangules_copy.clear();
        pTriangules_copy = pTriangules;
        pTriangules.clear();
        for(int iT = 0; iT < pTriangules_copy.size(); iT++)
        {
            Sea5kgTriangulationTriangle tr = pTriangules_copy[iT]->getTriangle();
            Sea5kgTriangulationLine L12(tr.p1, tr.p2);
            Sea5kgTriangulationLine L23(tr.p2, tr.p3);
            Sea5kgTriangulationLine L31(tr.p3, tr.p1);
            Sea5kgTriangulationPoint p_temp;

            if( L12.getPerpendicularToLine(tr.p3, p_temp) > a10 && 
                L23.getPerpendicularToLine(tr.p1, p_temp) > a10 && 
                L31.getPerpendicularToLine(tr.p2, p_temp) > a10)
                pTriangules.push_back(pTriangules_copy[iT]);                            
        }
        

        // m_pLogger->info(" print ");
        
        for(int iT = 0; iT < pTriangules.size(); iT++)
        {
            Sea5kgTriangulationTrianglePointer *tr = pTriangules[iT];
            Sea5kgTriangulationPoint p1(tr->p1->X, tr->p1->Y);
            Sea5kgTriangulationPoint p2(tr->p2->X, tr->p2->Y);
            Sea5kgTriangulationPoint p3(tr->p3->X, tr->p3->Y);

            Sea5kgTriangulationTriangle tr1(p1,p2,p3);
            m_triangles.push_back(tr1);            
        }
    };
};


void Sea5kgTriangulationTriangulator::triangulate() {
    clear_triangles();

//    step_first();
    step_first_alien();
/*
    for(int i = 0; i < m_areas.size(); i++)
    {
        m_nCurrArea = i;

        int nCountTriangles = m_areas[m_nCurrArea].getCountTriangles();
        double square = m_areas[m_nCurrArea].getSquare();
        double approximateTriangle = square / m_areas[m_nCurrArea].getCountTriangles();
        double a = sqrt((approximateTriangle * 4.0)/ sqrt(3));
        double h = 2 * approximateTriangle / a;
        double r = a * a * a / sqrt((3*a) * a * a * a );

        m_pLogger->info("All square: " + FloatToStr(square) + " px^2");
        m_pLogger->info("Approximate area of one triangle: " + FloatToStr(approximateTriangle) + " px^2");
        m_pLogger->info("a = " + FloatToStr(a) + " px");
        m_pLogger->info("h = " + FloatToStr(h) + " px");
        m_pLogger->info("r = " + FloatToStr(r) + " px");

        for(int i1 = 0; i1 < m_areas[m_nCurrArea].count(); i1++ )
        {                    
            int i2 = (i1+1) % m_areas[m_nCurrArea].count();
            Sea5kgTriangulationPoint p1 = m_areas[m_nCurrArea].getPoint(i1);
            Sea5kgTriangulationPoint p2 = triangulation::getNextPoint(p1, m_areas[m_nCurrArea].getPoint(i2), a);
            int nSize = m_triangles.size();
            netting nett(p1, p2, h, a, this);
            nett.processing();
            if(m_triangles.size() != nSize)
               break;    
        }
    };
*/
};
//---------------------------------------------------------------------------

void Sea5kgTriangulationTriangulator::clear_triangles()
{
    m_triangles.clear();
};
//---------------------------------------------------------------------------

void Sea5kgTriangulationTriangulator::clearAreas()
{
    m_areas.clear();
}
//---------------------------------------------------------------------------

bool Sea5kgTriangulationTriangulator::findTriangle(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2, Sea5kgTriangulationPoint p3)
{
    Sea5kgTriangulationTriangle tr(p1,p2,p3);
    for(int i = 0; i < m_triangles.size(); i++)
    {
        if(m_triangles[i].isEqual(tr))
            return true;
    };
    return false;
};

//---------------------------------------------------------------------------

bool Sea5kgTriangulationTriangulator::findNearPoint(Sea5kgTriangulationPoint p, Sea5kgTriangulationPoint &result, double r)
{
    Sea5kgTriangulationPoint res;
    double curr_len = -1;
    for(int i = 0; i < m_triangles.size(); i++)
    {
        Sea5kgTriangulationPoint p_buff;
        if(m_triangles[i].findNearPoint(p, p_buff, r))
        {
            if(curr_len == -1)
            {
                res = p_buff;
                curr_len = Sea5kgTriangulationLine(p, res).length();
            }
            else if(Sea5kgTriangulationLine(p, p_buff).length() < curr_len)
            {
                res = p_buff;
                curr_len = Sea5kgTriangulationLine(p, res).length();
            };
        };
    };

    if(curr_len == -1) return false;

    result = res;
    return true;
};

bool Sea5kgTriangulationTriangulator::findNearPointSide(Sea5kgTriangulationPoint p, Sea5kgTriangulationPoint &result, double r)
{
      {
        Sea5kgTriangulationPoint p_buff;
        if(m_areas[m_nCurrArea].findNearPointSide(p, p_buff, r))
        {
            result = p_buff;
            return true;
        };
    }

    {
        Sea5kgTriangulationPoint p_buff;
        for(unsigned int i2 = 0; i2 < m_areas.size(); i2++)
        {
            if(i2 != m_nCurrArea && m_areas[i2].findNearPointSide(p, p_buff, r))
            {
                result = p_buff;
                return true;
            }
        }
    }
    return false;
};

//---------------------------------------------------------------------------
// Sea5kgTriangulationNetting

Sea5kgTriangulationNetting::Sea5kgTriangulationNetting(const Sea5kgTriangulationPoint &p1, const Sea5kgTriangulationPoint &p2, double h, double a, Sea5kgTriangulationTriangulator *tr)
:
    m_h(h), m_a(a), m_tr(tr)
{
    Sea5kgTriangulationLine l1(p1,p2);
    if( l1.length() < 1) return;
    m_lines.push_back(l1);
}

//---------------------------------------------------------------------------

void Sea5kgTriangulationNetting::calcPoints(const Sea5kgTriangulationLine &L, Sea5kgTriangulationPoint &p4, Sea5kgTriangulationPoint &p5)
{
    Sea5kgTriangulationPoint p3 = L.getMiddlePoint();

     if(L.p1.X == L.p2.X)
     {
         p4.X = L.p1.X;
         p4.Y = p3.Y + m_h;

         p5.X = L.p1.X;
         p5.Y = p3.Y - m_h;
     }
     else if(L.p1.Y == L.p2.Y )
     {
         p4.Y = L.p1.Y;
         p4.X = p3.X + m_h;

         p5.Y = L.p1.Y;
         p5.X = p3.X - m_h;
     }
     else
     {
         double k1,k2;
         k1 = m_h / L.length();
         k2 = -k1;
         p4.X = p3.X + (L.p1.Y - L.p2.Y) * k1;
         p4.Y = p3.Y - (L.p1.X - L.p2.X) * k1;
         p5.X = p3.X + (L.p1.Y - L.p2.Y) * k2;
         p5.Y = p3.Y - (L.p1.X - L.p2.X) * k2;
     };

};

//---------------------------------------------------------------------------

void Sea5kgTriangulationNetting::processing()
{
  // clever add triangle as is
    while(m_lines.size() > 0)
    {
        Sea5kgTriangulationLine L = m_lines[0];
        m_lines.erase(m_lines.begin());

        Sea5kgTriangulationPoint p4, p5;
        calcPoints(L, p4, p5);


      bool bAdd4 = m_tr->addTriangleAsIs(L.p1,L.p2,p4, m_a, m_lines);
      bool bAdd5 = m_tr->addTriangleAsIs(L.p1,L.p2,p5, m_a, m_lines);          
    };

    /*
    std::vector<Sea5kgTriangulationTriangle> triangles = m_tr->getTriangles();

    
    // clever add triangle

    for(unsigned int i = 0; i < triangles.size(); i++)
    {
        Sea5kgTriangulationTriangle tr = triangles[i];
        m_lines.push_back(Sea5kgTriangulationLine(tr.p1,tr.p2));
        m_lines.push_back(Sea5kgTriangulationLine(tr.p2,tr.p3));
        m_lines.push_back(Sea5kgTriangulationLine(tr.p3,tr.p1));
    };

    while(m_lines.size() > 0)
    {
        Sea5kgTriangulationLine L = m_lines[0];
        m_lines.erase(m_lines.begin());

        Sea5kgTriangulationPoint p4, p5;
        calcPoints(L, p4, p5);

      bool bAdd4 = m_tr->addTriangle(L.p1,L.p2,p4, m_a, m_lines);
      bool bAdd5 = m_tr->addTriangle(L.p1,L.p2,p5, m_a, m_lines);          
    };
    */
};

/*
bool recours_netting(Sea5kgTriangulationPoint &p1, Sea5kgTriangulationPoint &p2, double h, double a, Sea5kgTriangulationTriangulator *tr)
{
    Sea5kgTriangulationLine l1(p1,p2);
    if( l1.length() < 1) return false;

    Sea5kgTriangulationPoint p3 = l1.getMiddlePoint();
    Sea5kgTriangulationPoint p4, p5;

    if(p1.X == p2.X)
    {
        p4.X = p1.X;
        p4.Y = p3.Y + h;

        p5.X = p1.X;
        p5.Y = p3.Y - h;
    }
    else if(p1.Y == p2.Y )
    {
        p4.Y = p1.Y;
        p4.X = p3.X + h;

        p5.Y = p1.Y;
        p5.X = p3.X - h;
    }
    else
    {
        double k1,k2;
        k1 = h / l1.length();
        k2 = -k1;
        p4.X = p3.X + (p1.Y - p2.Y) * k1;
        p4.Y = p3.Y - (p1.X - p2.X) * k1;
        p5.X = p3.X + (p1.Y - p2.Y) * k2;
        p5.Y = p3.Y - (p1.X - p2.X) * k2;
    };
//    m_pLogger->info();


    bool bAdd4 = tr->addTriangle(p1,p2,p4, a);
    bool bAdd5 = tr->addTriangle(p1,p2,p5, a);
        
    if(bAdd4) recours_netting(p1, p4, h, a, tr);
    if(bAdd5) recours_netting(p1, p5, h, a, tr);    
    if(bAdd4) recours_netting(p2, p4, h, a, tr);    
    if(bAdd5) recours_netting(p2, p5, h, a, tr);

    return false;
};
*/