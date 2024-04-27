#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
#include <list>

using namespace std;

enum type_of_lex
{
    LEX_NULL,      /*0*/
    LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_FOR, LEX_INT,  /*10*/
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TO, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE, LEX_GOTO, LEX_BREAK, /*22*/
    LEX_FIN,       /*23*/
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, /*31*/
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, /*39*/
    LEX_NUM,       /*40*/
    LEX_ID,        /*41*/
    POLIZ_LABEL,   /*42*/
    POLIZ_ADDRESS, /*43*/
    POLIZ_GO,      /*44*/
    POLIZ_FGO,     /*45*/
	POLIZ_DUP, /*46*/
	POLIZ_POP
};

class Lex
{
    type_of_lex  t_lex;
    int  v_lex;
public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0): t_lex (t), v_lex (v)  { }
    type_of_lex  get_type () const
    {
        return t_lex;
    }
    int  get_value () const
    {
        return v_lex;
    }
    friend ostream & operator<< (ostream &s, Lex l);
};

class Ident
{
    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
public:
    Ident()
    {
        declare = false;
        assign = false;
    }
    bool operator==(const string& s) const
    {
        return name == s;
    }
    Ident(const string n)
    {
        name = n;
        declare = false;
        assign = false;
    }
    string get_name () const
    {
        return name;
    }
    bool get_declare () const
    {
        return declare;
    }
    void put_declare ()
    {
        declare = true;
    }
    type_of_lex get_type () const
    {
        return type;
    }
    void put_type ( type_of_lex t )
    {
        type = t;
    }
    bool get_assign () const
    {
        return assign;
    }
    void put_assign ()
    {
        assign = true;
    }
    int  get_value () const
    {
        return value;
    }
    void put_value (int v)
    {
        value = v;
    }
};

vector<Ident> TID;

int put ( const string & buf )
{
    vector<Ident>::iterator k;

    if ( (k = find(TID.begin(), TID.end(), buf)) != TID.end())
        return k - TID.begin();
    TID.push_back(Ident(buf));
    return TID.size() - 1;
}

class Scanner
{
    static FILE * fp;
    static char c;
    static int look ( const string buf, const char ** list )
    {
        int i = 0;
        while (list[i])
        {
            if (buf == list[i])
                return i;
            ++i;
        }
        return 0;
    }
    static void gc ()
    {
        c = fgetc (fp);
    }
public:
    static const char * TW [], * TD [];
    Scanner ( const char * program )
    {
        fp = fopen ( program, "r" );
    }
    static Lex get_lex ();
};

char Scanner::c;
FILE * Scanner::fp;

const char *
Scanner::TW    [] = {"", "and", "begin", "bool", "do", "else", "end", "if", "false", "for", "int", "not", "or", "program",
                     "read", "then", "to", "true", "var", "while", "write", "goto", "break", NULL
                    };

const char *
Scanner::TD    [] = {"@", ";", ",", ":", ":=", "(", ")", "=", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", NULL};

Lex Scanner::get_lex ()
{
    enum state { H, IDENT, NUMB, COM, ALE, NEQ };
    int d, j;
    string buf;
    state CS = H;
    do
    {
        gc();
        switch(CS)
        {
        case H:
            if ( c==' ' || c == '\n' || c== '\r' || c == '\t' );
            else if ( isalpha(c) )
            {
                buf.push_back(c);                                                                      
                CS = IDENT;
            }
            else if ( isdigit(c) )
            {
                d = c - '0';
                CS = NUMB;
            }
            else if ( c== '{' )
            {
                CS = COM;
            }
            else if ( c== ':' || c== '<' || c== '>' )
            {
                buf.push_back(c);
                CS = ALE;
            }
            else if (c == '@')
                return Lex(LEX_FIN);
            else if (c == '!')
            {
                buf.push_back(c);
                CS = NEQ;
            }
            else
            {
                buf.push_back(c);
                if ( (j = look ( buf, TD)) )
                {
                    return Lex ( (type_of_lex)(j+(int)LEX_FIN), j );
                }
                else
                    throw c;
            }
            break;
        case IDENT:
            if ( isalpha(c) || isdigit(c) )
            {
                buf.push_back(c);
            }
            else
            {
                ungetc (c, fp);
                if ( (j = look (buf, TW)) )
                {
                    return Lex ((type_of_lex)j, j);
                }
                else
                {
                    j = put(buf);
                    return Lex (LEX_ID, j);
                }
            }
            break;
        case NUMB:
            if ( isdigit(c) )
            {
                d = d * 10 + (c - '0');
            }
            else
            {
                ungetc (c, fp);
                return Lex ( LEX_NUM, d);
            }
            break;
        case COM:
            if ( c == '}' )
            {
                CS = H;
            }
            else if (c == '@' || c == '{' )
                throw c;
            break;
        case ALE:
            if ( c== '=')
            {
                buf.push_back(c);
                j = look ( buf, TD );
                return Lex ( (type_of_lex)(j+(int)LEX_FIN), j);
            }
            else
            {
                ungetc (c, fp);
                j = look ( buf, TD );
                return Lex ( (type_of_lex)(j+(int)LEX_FIN), j);
            }
            break;
        case NEQ:
            if (c == '=')
            {
                buf.push_back(c);
                j = look ( buf, TD );
                return Lex ( LEX_NEQ, j );
            }
            else
                throw '!';
            break;
        }//end switch
    }
    while (true);
}

ostream & operator<< (ostream &s, Lex l)
{
    string t;
    if (l.t_lex <= 22)
        t = Scanner::TW[l.t_lex];
    else if (l.t_lex >= 23 && l.t_lex <= 39)
        t = Scanner::TD[l.t_lex-23];
    else if (l.t_lex == 40)
        t = "NUMB";
    else if (l.t_lex == 41)
        t = TID[l.v_lex].get_name();
    else if (l.t_lex == 42)
        t = "Label";
    else if(l.t_lex == 43)
        t = "Addr";
    else if (l.t_lex == 44)
        t = "!";
    else if (l.t_lex == 45)
        t = "!F";
	else if (l.t_lex==46)
	  t="dup";
	else if (l.t_lex==47)
	  t="pop";
    else
        throw l;
    s << '(' << t << ',' << l.v_lex << ");" << endl;
    return s;
}

template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
    i = st.top(); st.pop();
}


class Parser:Scanner
{
    static Lex l;
    static stack<string> stk;
    class S{public: void parse();};
    class Y{public: void parse();};
    class D{public: void parse();};
    class B{public: void parse();};
    class P{public: void parse();};

    class E{public: void parse();};
    class X{public: void parse();};
    class T{public: void parse();};
    class F{public: void parse();};

    static void  dec ( type_of_lex type);
    static void  check_id ();
	static void check_int();
    static void  check_op ();
    static void  check_not ();
    static void  eq_type ();
    static void  eq_bool ();
    static void  check_id_in_read ();

    static stack < int >           st_int;
    static stack < type_of_lex >   st_lex;



    struct temp {int pl0; int pl1;};

    static temp tmp;
    static stack <temp> tmp_st;



public:

    static vector <Lex> poliz;

    void analize();
    Parser (const char *s): Scanner(s) {}
};

Lex Parser::l;
stack<string> Parser::stk;

stack <int> Parser::st_int;
stack <type_of_lex> Parser::st_lex;

vector <Lex> Parser::poliz;



Parser::temp Parser::tmp;
stack <Parser::temp> Parser::tmp_st;



void Parser::D::parse()
    {
        //Dbeg:
        if(l.get_type() == LEX_ID){
	    st_int.push(l.get_value());
            l=get_lex();}
        else throw l;

        //D1:
        while(l.get_type() == LEX_COMMA)
        {
            l=get_lex();
            if(l.get_type() == LEX_ID){
		st_int.push(l.get_value());
                l=get_lex();}
            else throw l;
        }
        if(l.get_type() == LEX_COLON)
            l=get_lex();
        else throw l;
        //D2:
        if(l.get_type() == LEX_INT )
	{
	    dec(LEX_INT);
            l=get_lex();
	}
	else if(l.get_type()== LEX_BOOL)
	{
	    dec(LEX_BOOL);
            l=get_lex();
	}

        else throw l;
        //Dend:
    }


void Parser::S::parse()
    {
        switch(l.get_type())
        {
        case LEX_ID:
        {

	    check_id();
	    poliz.push_back(Lex(POLIZ_ADDRESS,l.get_value()));
            l=get_lex();
            // S1
            if(l.get_type() != LEX_ASSIGN)
                throw l;
            l = get_lex();
            //S2
            stk.push("(SE1");
            E().parse();
            if(stk.top() == "(SE1")
                stk.pop();
            else
                throw l;

	    eq_type();
	    poliz.push_back(Lex(LEX_ASSIGN));
        }
        break;
        case LEX_IF:
        {
            l=get_lex();
            //S3
            stk.push("(SE2");
            E().parse();
            if(stk.top() == "(SE2")
                stk.pop();
            else throw l;
	    eq_bool();
	    tmp.pl0 = poliz.size();
	    poliz.push_back(Lex());
	    poliz.push_back(Lex(POLIZ_FGO));
            //S4
            if(l.get_type() == LEX_THEN)
            {
                l = get_lex();
                //S5
                stk.push("(SS1");
		tmp_st.push(tmp);
                S().parse();
                if(stk.top() == "(SS1")
                {
                    stk.pop();
                }
                else throw l;
		from_st(tmp_st,tmp);
		tmp.pl1 = poliz.size();
		poliz.push_back(Lex());
		poliz.push_back(Lex(POLIZ_GO));
		poliz[tmp.pl0]= Lex(POLIZ_LABEL,poliz.size());
                //S6

                if(l.get_type() == LEX_ELSE)
                {
                    l = get_lex();
                        //S7
                        stk.push("(SS2");
			tmp_st.push(tmp);
                    S().parse();
                    if(stk.top() == "(SS2")
                    {
                        stk.pop();
                    }
                    else throw l;
                    tmp = tmp_st.top();
		    tmp_st.pop();
                    poliz[tmp.pl1] = Lex(POLIZ_LABEL,poliz.size());
                }
                else throw l;
            }
            else throw l;
        }
        break;
        case LEX_WHILE:
        {
            l=get_lex();
            //S8
            stk.push("(SE3");
	    tmp.pl0 = poliz.size();
            E().parse();
            if(stk.top() == "(SE3")
            {
                stk.pop();
            }
            else throw l;

	    eq_bool();
	    tmp.pl1 = poliz.size();
	    poliz.push_back(Lex());
	    poliz.push_back(Lex(POLIZ_FGO));
	    //S9
            if(l.get_type() == LEX_DO)
            {
                l = get_lex();
                //S10
                stk.push("(SS3");
		tmp_st.push(tmp);
                S().parse();
                if(stk.top() == "(SS3")
                {
                    stk.pop();
		    from_st(tmp_st,tmp);
		    poliz.push_back(Lex(POLIZ_LABEL,tmp.pl0));
		    poliz.push_back(Lex(POLIZ_GO));
		    poliz[tmp.pl1] = Lex(POLIZ_LABEL,poliz.size());
                }
                else throw l;
            }
            else throw l;
        }
        break;
        case LEX_READ:
        {
            l=get_lex();
            //S11
            if(l.get_type() == LEX_LPAREN)
            {
                l = get_lex();
                if(l.get_type() == LEX_ID)
                {//S12
		    check_id_in_read();
		    poliz.push_back(Lex(POLIZ_ADDRESS,l.get_value()));
                    l = get_lex();
                    //S13
                    if(l.get_type() == LEX_RPAREN)
                    {
			poliz.push_back(Lex(LEX_READ));
                        l = get_lex();
                    }
                    else throw l;
                }
                else throw l;
            }
            else throw l;
        }
        break;
        case LEX_WRITE:
        {
            l=get_lex();
            //S14
            if(l.get_type() == LEX_LPAREN)
            {
                l = get_lex();
                //S15
                stk.push("(SE4");
                E().parse();
                if(stk.top() == "(SE4")
                {
                    stk.pop();
                }
                else throw l;
		st_lex.pop();
                //S16
                if(l.get_type() != LEX_RPAREN)
                    throw l;
		poliz.push_back(Lex(LEX_WRITE));
                l = get_lex();
            }
        }
        break;

	case LEX_FOR:
	{
	  int p;
	  l=get_lex();
	  //Sf2
	  if (l.get_type()==LEX_ID)
	  {
	    check_id();
	    poliz.push_back(Lex(POLIZ_ADDRESS, l.get_value()));
	    p=l.get_value();
	    l=get_lex();
	    //Sf3
	    if (l.get_type()==LEX_ASSIGN)
	    {
	      l=get_lex();
	      stk.push("(FF3");
	      E().parse();
	      if (stk.top()=="(FF3")
	      {
	        stk.pop();
	      }
	      else throw l;
	      //Sf4
	      check_int();
              st_lex.push(LEX_INT);
              eq_type();
	      poliz.push_back(Lex(LEX_ASSIGN));
	      if (l.get_type()==LEX_TO)
	      {
	        l=get_lex();
	      }
	      else throw l;
	      //Sf5

	      stk.push("(SS5");
	      E().parse();
	      //Sf6
	      if (stk.top()=="(SS5")
              {
                stk.pop();
              }
              else throw l;
	      if (l.get_type()==LEX_DO)
	      {
	        l=get_lex();
	      }
	      else throw l;
	      check_int();
	      tmp.pl0=poliz.size();

	      poliz.push_back(Lex(POLIZ_DUP));
	      poliz.push_back(Lex(LEX_ID, p));

	      poliz.push_back(Lex(LEX_GEQ));
	      tmp.pl1=poliz.size();
	      poliz.push_back(Lex());
	      poliz.push_back(Lex(POLIZ_FGO));
	      tmp_st.push(tmp);
	      //Sf7

	tmp_st.push(tmp);
	      stk.push("(SS7");
	      S().parse();
              if (stk.top()=="(SS7")
              {
                stk.pop();
              }
	      else throw l;
	from_st(tmp_st, tmp);
	      poliz.push_back(Lex(POLIZ_ADDRESS, p));

	      poliz.push_back(Lex(LEX_ID, p));
	      poliz.push_back(Lex(LEX_NUM, 1));
	      poliz.push_back(Lex(LEX_PLUS));
	      poliz.push_back(Lex(LEX_ASSIGN));
	      poliz.push_back(Lex(POLIZ_LABEL, tmp.pl0));
	      poliz.push_back(Lex(POLIZ_GO));
	      poliz[tmp.pl1]=Lex(POLIZ_LABEL, poliz.size());
	      poliz.push_back(Lex(POLIZ_POP));
	    }
	    else throw l;
	  }
	  else throw l;
	}
	break;
        default:
        {
            stk.push("(SB");
            B().parse();
            if(stk.top() == "(SB")
            {
                stk.pop();

            }
            else throw l;

        }

        }//Send

    }

void Parser::E::parse()
    {
        //Ebeg
        stk.push("(EX1");
        X().parse();
        if(stk.top() == "(EX1")
        {
            stk.pop();
        }
        else throw l;
        //E1
        switch (l.get_type())
        {
        case (LEX_EQ):
        case (LEX_LEQ):
        case (LEX_GEQ):
        case (LEX_NEQ):
        case (LEX_GTR):
        case (LEX_LSS):
        {
	    st_lex.push(l.get_type());
            l = get_lex();
            //E2
            stk.push("(EX2");
            X().parse();
            if(stk.top() == "(EX2")
            {
                stk.pop();
            }
            else throw l;
	    check_op();
        }
        break;
        default:
        {
            break ;
        }
        }//Eend
    }


void Parser::X::parse()
    {
        //Xbeg
        stk.push("(XT1");
        T().parse();
        if(stk.top() == "(XT1")
        {
            stk.pop();
        }
        else throw l;
        while(l.get_type() == LEX_OR || l.get_type() == LEX_PLUS || l.get_type() == LEX_MINUS)
        {
            //X1
	    st_lex.push(l.get_type());
            l = get_lex();
            stk.push("(XT2");
            T().parse();
            if(stk.top() == "(XT2")
            {
                stk.pop();
            }
            else throw l;
	    check_op();
        }
        //Xend
    }

void Parser::T::parse()
    {
        //Tbeg
        stk.push("(TF1");
        F().parse();
        if(stk.top() == "(TF1")
        {
            stk.pop();
        }
        else throw l;
        while(l.get_type() == LEX_AND || l.get_type() == LEX_TIMES || l.get_type() == LEX_SLASH)
        {
	    st_lex.push(l.get_type());
            //T1
            l = get_lex();
            stk.push("(TF2");
            F().parse();
            if(stk.top() == "(TF2")
            {
                stk.pop();
            }
            else throw l;
	    check_op();
        }//Tend
    }


void Parser::F::parse()
    {
        switch(l.get_type())
        {
        case (LEX_LPAREN):
        {
            l = get_lex();
            //F1
            stk.push("(FE");
            E().parse();
            if(stk.top() == "(FE")
            {
                stk.pop();
            }
            else throw l;
            //F2
            if(l.get_type() == LEX_RPAREN)
            {
                l = get_lex();
            }
            else throw l;
        }
        break;
        case (LEX_NOT):
        {
            l = get_lex();
            // F3
            stk.push("(FF");
            F().parse();
            if(stk.top() == "(FF")
            {
                stk.pop();
            }
            else throw l;
	    check_not();
        }
        break;
        case (LEX_ID):
        {
	    check_id();
	    poliz.push_back(l);
	    l = get_lex();
        }
        break;
        case (LEX_NUM):
        {
	    st_lex.push(LEX_INT);
	    poliz.push_back(l);
            l = get_lex();
        }
        break;
        case (LEX_TRUE) :
        case (LEX_FALSE):
        {
	    st_lex.push(LEX_BOOL);
	    poliz.push_back(l);
            l = get_lex();
        }
        break;
        default:
        {
            throw l;
        }
        }
        //Fend
    }


void Parser::Y::parse()
    {
        //Ybeg:
        if(l.get_type() == LEX_VAR)
            l=get_lex();
        else throw l;
        //Y1:
        stk.push("(YD");
        //Dbeg:
        D().parse();
        //Dend:
        if(stk.top() == "(YD") stk.pop();
        else throw l;
        //Yend:
        while(l.get_type() == LEX_COMMA)
        {
            l=get_lex();
            //Y1:
            stk.push("(YD");
            //Dbeg:
            D().parse();
            //Dend:
            if(stk.top() == "(YD") stk.pop();
            else throw l;
            //Yend:
        }
    }

void Parser::B::parse()
    {
        //Bbeg:
        if(l.get_type() == LEX_BEGIN)
            l=get_lex();
        else throw l;
        //B1:
        stk.push("(BS");
        //Sbeg:
        S().parse();
        //Send:
        if(stk.top() == "(BS")
	{
	  stk.pop();
	}

        else throw l;
        //B2:
        while(l.get_type() == LEX_SEMICOLON)
        {
            l=get_lex();
            //B1:
            stk.push("(BS");
            //Sbeg:
            S().parse();
            //Send:
            if(stk.top() == "(BS") stk.pop();
            else throw l;
        }

        if(l.get_type() == LEX_END)
            l=get_lex();
        else throw l;
    }


void Parser::P::parse()
    {
        //Pbeg:
        if(l.get_type() == LEX_PROGRAM)

            l=get_lex();
        else throw l;
        //P1:
        stk.push("(PY");
        //Ybeg:
        Y().parse();
        //Yend:
        if(stk.top() == "(PY") stk.pop();
        else throw l;
        //P2:
        if(l.get_type() == LEX_SEMICOLON)
        {
            l=get_lex();
            //P3:
            stk.push("(PB");
            //Bbeg:
            B().parse();
            //Bend:
            if(stk.top() == "(PB") stk.pop();
            else throw l;
        }
        else throw l;
        //P4:
        if(l.get_type() != LEX_FIN) throw l;
    }


void Parser::dec ( type_of_lex type )
{
    int i;
    while ( !st_int.empty () ) {
        from_st ( st_int, i );
        if ( TID[i].get_declare () )
            throw "twice";
        else {
            TID[i].put_declare ();
            TID[i].put_type ( type );
        }
    }
}

void Parser::check_id ()
{
    if ( TID[l.get_value()].get_declare() )
        st_lex.push ( TID[l.get_value()].get_type () );
    else
	{cout << "check_id : l = " << l << endl;
        throw "not declared";}
}

void Parser::check_int()
{
	    if ( st_lex.top () != LEX_INT)
	      throw "expression is not integer";
    	    st_lex.pop ();

}

void Parser::check_op ()
{
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;

    from_st ( st_lex, t2 );
    from_st ( st_lex, op );
    from_st ( st_lex, t1 );

    if ( op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH )
        r = LEX_INT;
    if ( op == LEX_OR || op == LEX_AND )
        t = LEX_BOOL;
    if ( t1 == t2  &&  t1 == t )
        st_lex.push (r);
    else
        throw "wrong types are in operation";
    poliz.push_back (Lex (op) );
}

void Parser::check_not () {
    if (st_lex.top() != LEX_BOOL)
        throw "wrong type is in not";
   // else
    poliz.push_back ( Lex (LEX_NOT) );
}

void Parser::eq_type () {
    type_of_lex t;
    from_st ( st_lex, t );
    if ( t != st_lex.top () )
        throw "wrong types are in :=";
    st_lex.pop();
}

void Parser::eq_bool () {
    if ( st_lex.top () != LEX_BOOL )
        throw "expression is not boolean";
    st_lex.pop ();
  }

void Parser::check_id_in_read () {
    if ( !TID [l.get_value()].get_declare() )
	{
	    cout << "check_id_in_read : l = "<< l << endl;
            throw "not declared";
	}
}


void Parser::analize()
{
    l=get_lex();
    stk.push("(start");
    P().parse();
    if(stk.top() == "(start") stk.pop();
    else throw l;

};



class Executer {
public:
    void execute ( vector<Lex> & poliz );
};

void Executer::execute ( vector<Lex> & poliz ) {
    Lex pc_el;
    stack < int > args;
    int i, j, index = 0, size = poliz.size();
    while ( index < size ) {
        pc_el = poliz [ index ];
        switch ( pc_el.get_type () ) {
            case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case POLIZ_ADDRESS: case POLIZ_LABEL:
                args.push ( pc_el.get_value () );
                break;

            case LEX_ID:
                i = pc_el.get_value ();
                if ( TID[i].get_assign () )
		{
                  args.push ( TID[i].get_value () );
                  break;
                }
                else
                  throw "POLIZ: indefinite identifier";

            case LEX_NOT:
                from_st ( args, i );
                args.push( !i );
                break;

            case LEX_OR:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j || i );
                break;

            case LEX_AND:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j && i );
                break;

            case POLIZ_GO:
                from_st ( args, i );
                index = i - 1;
                break;

            case POLIZ_FGO:
                from_st ( args, i );
                from_st ( args, j );
                if ( !j ) index = i - 1;
                break;

	    case POLIZ_DUP:
	      from_st(args, i);
	      args.push(i);
		args.push(i);
	      break;

	    case POLIZ_POP:
		args.pop();
	      break;

            case LEX_WRITE:
                from_st ( args, j );
                cout << j << endl;
                break;

            case LEX_READ:
                int k;
                from_st ( args, i );
                if ( TID[i].get_type () == LEX_INT ) {
                    cout << "Input int value for " << TID[i].get_name () << endl;
                    cin >> k;
                }
                else {
                    string j;
                    while (1) {
                        cout << "Input boolean value (true or false) for" << TID[i].get_name() << endl;
                        cin >> j;
                        if ( j != "true" && j != "false" ) {
                            cout << "Error in input:true/false" << endl;
                            continue;
                        }
                        k = ( j == "true" ) ? 1 : 0;
                        break;
                    }
                }
                TID[i].put_value (k);
                TID[i].put_assign ();
                break;

            case LEX_PLUS:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( i + j );
                break;

            case LEX_TIMES:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( i * j );
                break;

            case LEX_MINUS:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j - i );
                break;

            case LEX_SLASH:
                from_st ( args, i );
                from_st ( args, j );
                if (!i) {
                    args.push ( j / i );
                    break;
                }
                else
                    throw "POLIZ:divide by zero";

            case LEX_EQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( i == j );
                break;

            case LEX_LSS:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j < i );
                break;

            case LEX_GTR:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j > i );
                break;

            case LEX_LEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j <= i );
                break;

            case LEX_GEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j >= i );
                break;

            case LEX_NEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j != i );
                break;

            case LEX_ASSIGN:
                from_st ( args, i );
                from_st ( args, j );
                TID[j].put_value (i);
                TID[j].put_assign ();
                break;

            default:
	        cerr<<pc_el<<endl;
                throw "POLIZ: unexpected elem";
        }
        ++index;
    };
}

class Interpretator {
    Parser   pars;
    Executer E;
public:
    Interpretator (const char* program ): pars (program) {}
    void     interpretation ();
};

void Interpretator::interpretation () {
    pars.analize ();
    E.execute ( pars.poliz );
}

int main()
{
    try
    {
	  Interpretator I("prog.txt");
	  I.interpretation();
	  cout << "YAY" << endl;
    }
    catch(char c)
    {
        cout << "unexpected symbol " << c << endl;
    }
    catch(Lex l)
    {
        cout << "unexpected lexeme " << l << endl;
    }
    catch(const char * s)
    {
	cout << "ERROR : " << s << endl;
    }
    return 0;
}
