void dumphex(const vector<char>& v)
{
    const int N = 16;
    const char hex[] = "0123456789ABCDEF";
    char buf[N*4+5+2];
    for (int i = 0; i < v.size(); ++i)
    {
        int n = i % N;
        if (n == 0)
        {
            if (i)
                puts(buf);
            memset(buf, 0x20, sizeof(buf));
            buf[sizeof(buf) - 2] = '\n';
            buf[sizeof(buf) - 1] = '\0';
        }
        unsigned char c = (unsigned char)v[i];
        buf[n*3+0] = hex[c / 16];
        buf[n*3+1] = hex[c % 16];
        buf[3*N+5+n] = (c>=' ' && c<='~') ? c : '.';
    }
    puts(buf);
}