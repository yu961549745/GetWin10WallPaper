package org.yjt.win10pic;

import java.io.File;
import java.io.FileInputStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.security.DigestInputStream;
import java.security.MessageDigest;

/**
 * 获取文件的MD5
 * 
 * @author yjt
 */
public class FileMD5Getter {
	public static String getMD5(File f) throws Exception {
		MessageDigest md5 = MessageDigest.getInstance("MD5");
		DigestInputStream in = new DigestInputStream(new FileInputStream(f),
				md5);
		byte[] buffer = new byte[1024];
		while (in.read(buffer) >= 0) {
		}
		in.close();
		return byte2hex(md5.digest());
	}

	public static String byte2hex(byte[] bs) {
		StringWriter sw = new StringWriter(bs.length * 2);
		PrintWriter w = new PrintWriter(sw);
		for (byte b : bs) {
			w.printf("%02X", b);
		}
		w.close();
		return sw.toString();
	}
}
