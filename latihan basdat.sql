-- soal 1
CREATE DATABASE komang;

use komang;
CREATE TABLE departments (
department_id INT PRIMARY KEY,
department_name VARCHAR(100)
);

CREATE TABLE employees ( --employees adalah nama tabel untuk menyimpan data pegawai
employee_id INT PRIMARY KEY,
employee_name VARCHAR(100),
department_id INT,
gaji DECIMAL(15,2),
FOREIGN KEY (department_id) REFERENCES departments(department_id) -- fungsi references buat mastikan nilai di satu tabel harus sesuai dengan nilai yang ada di tabel lain
);

INSERT INTO departments (department_id, department_name)
VALUES 
(1, 'Teknik'),
(2, 'Pengembangan Perangkat Lunak'),
(3, 'Sumber Daya Manusia'),
(4, 'Operasional'),
(5, 'Penjualan'),
(6, 'Pemasaran'),
(7, 'Media Sosial'),
(8, 'Rekrutmen'),
(9, 'Keuangan'),
(10, 'Penjualan Ritel');

INSERT INTO employees (employee_id, employee_name, department_id, gaji)
VALUES 
(1, 'Andi', 1, 8500000),
(2, 'Budi', 1, 15000000),
(3, 'Citra', 1, 10000000),
(4, 'Dewi', 2, 9000000),
(5, 'Eko', 2, 11000000),
(6, 'Fajar', 2, 9500000),
(7, 'Gina', 3, 6500000),
(8, 'Hadi', 3, 13000000),
(9, 'Ika', 4, 14000000),
(10, 'Joko', 5, 12500000),
(11, 'Kiki', 6, 12000000),
(12, 'Lina', 7, 8000000),
(13, 'Maya', 8, 7500000),
(14, 'Nina', 9, 7000000),
(15, 'Omar', 10, 7000000);

SELECT 
d.department_name,
COUNT(e.employee_id) AS employee_count,
SUM(e.gaji) AS total_gaji,
AVG(e.gaji) AS rata_rata_gaji,
MIN(e.gaji) AS min_gaji,
MAX(e.gaji) AS max_gaji
FROM employees e
JOIN departments d ON e.department_id = d.department_id -- fungsi join untuk menggabungkan data dari dua atau lebih tabel berdasarkan kolom yang saling berhubungan.
GROUP BY d.department_name
ORDER BY total_gaji DESC; -- fungsi order by untuk mengurutkan hasil query berdasarkan satu atau lebih kolom.



-- hapus database lama kalau sudah ada
DROP DATABASE IF EXISTS komang;
-- soal 2
-- buat database baru
CREATE DATABASE komang;
USE komang;

-- buat tabel proyek
CREATE TABLE proyek (
proyek_id INT PRIMARY KEY,
proyek_name VARCHAR(100),
anggaran DECIMAL(15,2)
);

-- buat tabel pegawai
CREATE TABLE pegawai (
pegawai_id INT PRIMARY KEY,
nama_depan VARCHAR(50),
nama_belakang VARCHAR(50),
proyek_id INT,
FOREIGN KEY (proyek_id) REFERENCES proyek(proyek_id)
);

-- isi data proyek
INSERT INTO proyek (proyek_id, proyek_name, anggaran)
VALUES
(1, 'Membangun Jembatan', 2500000),
(2, 'Membangun Timbangan Sawit', 3000000),
(3, 'Membuka Toko Sembako', 1000000),
(4, 'Memperbaiki Jalan Raya', 1500000);

-- isi data pegawai
INSERT INTO pegawai (pegawai_id, nama_depan, nama_belakang, proyek_id)
VALUES
(1, 'Komang', 'Kariani', 2),
(2, 'Hizkia', 'Husai', 4),
(3, 'Anggun', 'Setiyawati', 1),
(4, 'Putu', 'Nita', 3),
(5, 'Nandhita', 'Eka', 3);

SELECT 
p.nama_depan,
p.nama_belakang,
pr.proyek_name,
pr.anggaran
FROM 
pegawai p
JOIN 
proyek pr ON p.proyek_id = pr.proyek_id
ORDER BY 
pr.anggaran ASC;

-- soal 3
DROP DATABASE IF EXISTS komang;
CREATE DATABASE komang;
USE komang;

CREATE TABLE departemen (
departemen_id INT PRIMARY KEY,
nama_departemen VARCHAR(100)
);

CREATE TABLE pegawai (
pegawai_id INT PRIMARY KEY,
nama_depan VARCHAR(50),
nama_belakang VARCHAR(50),
email VARCHAR(100),
departemen_id INT,
FOREIGN KEY (departemen_id) REFERENCES departemen(departemen_id)
);

INSERT INTO departemen (departemen_id, nama_departemen)
VALUES
(1, 'Teknik'),
(2, 'Pertambangan'),
(3, 'Industri'),
(4, 'Sistem Informasi'),
(5, 'Geologi');

INSERT INTO pegawai (pegawai_id, nama_depan, nama_belakang, email, departemen_id)
VALUES
(1, 'Komang', 'Kariani', 'komang.kariani@expamle.com', 1),
(2, 'Hizkia', 'Husai', 'hizkia.husai@example.com', 2),
(3, 'Anggun', 'Setiyawati', 'anggun.setiyawati@example.com', 3),
(4, 'Putu', 'Nita', 'putu.nita@example.com', 4),
(5, 'Nandhita', 'Eka', 'nandhita.eka@example.com', 5);

SELECT 
p.nama_depan,
p.nama_belakang,
p.email,
d.nama_departemen
FROM
pegawai p 
JOIN
departemen d ON p.departemen_id = d.departemen_id
WHERE
p.email NOT LIKE '@%mail.com'
ORDER BY
p.nama_depan ASC;