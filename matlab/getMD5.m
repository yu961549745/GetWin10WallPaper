function s = getMD5(fname)
if ~ischar(fname)
    error('���������ļ���');
end
if ~exist(fname,'file')
    error('�ļ�������: %s',fname);
end
x=mMD5(fname);
s=sprintf('%02X',x);
end