#pragma once

#include <cmath>
#include <string>
#include <vector>

class Sea5kgTriangulationPoint {
    public:
        Sea5kgTriangulationPoint();
        Sea5kgTriangulationPoint(double X, double Y);
        Sea5kgTriangulationPoint(const Sea5kgTriangulationPoint &p);
        void incCounter();
        int getCounterValue();

        const double &getX() const;
        const double &getY() const;
        int getXint() const;
        int getYint() const;

        double calcLength(const Sea5kgTriangulationPoint &p) const;

        void operator += (const Sea5kgTriangulationPoint &p);
        const Sea5kgTriangulationPoint operator / (const int n);
        void operator = (const Sea5kgTriangulationPoint &p);
        bool operator == (const Sea5kgTriangulationPoint &p) const;
        bool operator != (const Sea5kgTriangulationPoint &p);
        std::string toString();
        double X,Y;

    private:
        int m_nCounter;
};

Sea5kgTriangulationPoint getNextPoint(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2, double length);

class Sea5kgTriangulationLine {
    public:
        Sea5kgTriangulationLine();
        Sea5kgTriangulationLine(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2);

        bool hasPoint(const Sea5kgTriangulationPoint &p3);
        const double getPerpendicularToLine(const Sea5kgTriangulationPoint &p3, Sea5kgTriangulationPoint &res);

        Sea5kgTriangulationPoint p1, p2;
        bool hasIntersection(Sea5kgTriangulationLine &l, Sea5kgTriangulationPoint &result);
        double length() const;
        Sea5kgTriangulationPoint getMiddlePoint() const;

        private:
            bool hasIntersection2(Sea5kgTriangulationLine &l, Sea5kgTriangulationPoint &result);
            bool hasIntersection3(Sea5kgTriangulationLine &l, Sea5kgTriangulationPoint &result);
};

class Sea5kgTriangulationTriangle {
    public:
        Sea5kgTriangulationTriangle();
        Sea5kgTriangulationTriangle(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2, Sea5kgTriangulationPoint p3);

        // TODO hide
        Sea5kgTriangulationPoint p1, p2, p3;

        double getSquare();
        bool findNearPoint(Sea5kgTriangulationPoint p, Sea5kgTriangulationPoint &res, double r);
        // void paint(TImage *img);
        bool hasIntersection(const Sea5kgTriangulationLine &l);
        bool hasPoint(const Sea5kgTriangulationPoint &p);
        bool hasTop(const Sea5kgTriangulationPoint &p) const;
        std::string toString();
        // void triangle::fromString(UnicodeString str);
        bool operator == (const Sea5kgTriangulationTriangle &tr);
        void operator = (const Sea5kgTriangulationTriangle &tr);
        bool isEqual(const Sea5kgTriangulationTriangle &tr);

    private:
};

class Sea5kgTriangulationTrianglePointer {
    public:
        Sea5kgTriangulationTrianglePointer();

        Sea5kgTriangulationPoint* p1;
        Sea5kgTriangulationPoint* p2;
        Sea5kgTriangulationPoint* p3;

        Sea5kgTriangulationTriangle getTriangle();
        Sea5kgTriangulationPoint get_p1();
        Sea5kgTriangulationPoint get_p2();
        Sea5kgTriangulationPoint get_p3();

    private:
};

class Sea5kgTriangulationArea {
    public:
        Sea5kgTriangulationArea(std::string sAreaId);
        std::string getId();
        void addPoint(int X, int Y) { m_vPoints.push_back(Sea5kgTriangulationPoint(X,Y)); };
        void clear() { m_vPoints.clear(); };
        int count() const { return m_vPoints.size(); };
        const Sea5kgTriangulationPoint &getPoint(int i) const { return m_vPoints[i]; };
        void setCountTriangles(int nCountTriangles)  { m_nCountTriangles = nCountTriangles; };
        unsigned int getCountTriangles() const { return m_nCountTriangles; };
        bool hasPoint(const Sea5kgTriangulationPoint &p1) const;
        const double getPerpendicularToLine(const Sea5kgTriangulationPoint &p3, Sea5kgTriangulationPoint &res);
        bool hasLine(const Sea5kgTriangulationLine &l) const;
        bool hasIntersections(Sea5kgTriangulationLine l, std::vector<Sea5kgTriangulationPoint> &result);
        bool hasIntersections(Sea5kgTriangulationArea &l, std::vector<Sea5kgTriangulationPoint> &result);
        // void paint(TImage *img);
        double getSquare();
        Sea5kgTriangulationPoint getMiddlePoint();
        bool findNearPointSide(const Sea5kgTriangulationPoint &p, Sea5kgTriangulationPoint &res, double len);
        bool getMinMaxXY(double &minX, double &maxX, double &minY, double &maxY);
        
    private:
        std::string m_sAreaId;
        std::vector<Sea5kgTriangulationPoint> m_vPoints;
        unsigned int m_nCountTriangles;
};

class Sea5kgTriangulationTriangulator {
    public:
        Sea5kgTriangulationTriangulator();
        bool addTriangle( 
                Sea5kgTriangulationPoint p1, 
                Sea5kgTriangulationPoint p2, 
                Sea5kgTriangulationPoint p3, 
                double a, 
                std::vector<Sea5kgTriangulationLine> &m_lines
        );

        bool addTriangleAsIs(
            Sea5kgTriangulationPoint p1, 
            Sea5kgTriangulationPoint p2, 
            Sea5kgTriangulationPoint p3, 
            double a,
            std::vector<Sea5kgTriangulationLine> &m_lines
        );

        void addArea(Sea5kgTriangulationArea ar);
        std::vector<Sea5kgTriangulationArea> &getAreas();
        std::vector<Sea5kgTriangulationTriangle> &getTriangles();
        void triangulate();

        void triangulate_add_triangles();
        void triangulate_resizing();
        void triangulate_remove_triangles();

        void clear_triangles();
        void clearAreas();

    private:
        
        std::vector<Sea5kgTriangulationArea> m_areas;
        std::vector<Sea5kgTriangulationTriangle> m_triangles;
        int m_nCurrArea;
        int m_nDivisor;
        double m_nInfelicity;

        // triangulation::logger *m_pLogger;
        void step_first();
        void step_first_alien();

        void findPoints(
            const Sea5kgTriangulationPoint &p1, 
            const std::vector<Sea5kgTriangulationPoint> &points, 
            double a, 
            std::vector<Sea5kgTriangulationTriangle> &triangles
        );


        //
        bool findTriangle(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2, Sea5kgTriangulationPoint p3);
        bool findNearPoint(Sea5kgTriangulationPoint p, Sea5kgTriangulationPoint &result, double r);
        bool findNearPointSide(Sea5kgTriangulationPoint p, Sea5kgTriangulationPoint &result, double r);

        bool hasCurrentArea(Sea5kgTriangulationPoint p1);
        bool hasCurrentArea(Sea5kgTriangulationPoint p1, Sea5kgTriangulationPoint p2);
        bool hasIntersectionWithOtherTriangles(
            const Sea5kgTriangulationTriangle &tr,  
            Sea5kgTriangulationTriangle &result,
            Sea5kgTriangulationPoint &p_result
        );

        void fillArray(const Sea5kgTriangulationPoint &p, std::vector<Sea5kgTriangulationPoint> &p_arr, double r);
};

class Sea5kgTriangulationNetting
{
    public:
        Sea5kgTriangulationNetting(const Sea5kgTriangulationPoint &p1, const Sea5kgTriangulationPoint &p2, double h, double a, Sea5kgTriangulationTriangulator *tr);
        void processing();
        void calcPoints(const Sea5kgTriangulationLine &L, Sea5kgTriangulationPoint &p4, Sea5kgTriangulationPoint &p5);
    private:
        std::vector<Sea5kgTriangulationLine> m_lines;
        double m_h; 
        double m_a; 
        Sea5kgTriangulationTriangulator *m_tr;
};

//    bool recours_netting(Sea5kgTriangulationPoint &p1, Sea5kgTriangulationPoint &p2, double h, double a, Sea5kgTriangulationTriangulator *tr);
