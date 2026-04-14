#ifndef __Shape__
#define __Shape__

#define GRID_ROWS 20
#define GRID_COLS 25
#define CELL_SIZE 22
#define MAX_SNAKE 400

class Shape
{
    public:
        void Shape_genrate();
        static Shape* Instance()
        {
            if(s_pInstance == 0)
            {
                s_pInstance = new Shape();
                return s_pInstance;
            }
            return s_pInstance;
        }

    private:
        Shape() {}
        static Shape* s_pInstance;
};

typedef Shape TheShape;

#endif /* defined(__Shape__) */
