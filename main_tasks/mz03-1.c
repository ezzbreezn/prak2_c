int
bitcount(STYPE value)
{
    UTYPE uvalue = value;
    int ans = 0;
    while (uvalue) {
        ans += uvalue & 1;
        uvalue >>= 1;
    }
    return ans;
}

