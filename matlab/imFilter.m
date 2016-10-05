function b = imFilter(f)
sz=mexGetJpgSize(f);
if isempty(sz)
    b=false;
    return;
end
b=sz(1)<sz(2);
end