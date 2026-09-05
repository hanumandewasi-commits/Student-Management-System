const seedStudents = [{roll:25,name:'Hanuman',city:'Jaipur',age:20,mobile:'7340017521',percentage:86,branch:'CSE'}];
let students = JSON.parse(localStorage.getItem('campusStudents') || 'null') || seedStudents;
let currentRole = 'admin';
let editingRoll = null;

const $ = selector => document.querySelector(selector);
const esc = value => String(value).replace(/[&<>'"]/g, char => ({'&':'&amp;','<':'&lt;','>':'&gt;',"'":'&#39;','"':'&quot;'}[char]));
const initials = name => name.split(' ').map(part => part[0]).slice(0, 2).join('').toUpperCase();
const save = () => localStorage.setItem('campusStudents', JSON.stringify(students));
const showToast = message => { const toast = $('#toast'); toast.textContent = message; toast.classList.add('show'); setTimeout(() => toast.classList.remove('show'), 2200); };

function render() {
  const total = students.length;
  const average = total ? students.reduce((sum, student) => sum + Number(student.percentage), 0) / total : 0;
  const branches = [...new Set(students.map(student => student.branch.toUpperCase()))];
  $('#totalStudents').textContent = total;
  $('#averageScore').textContent = average.toFixed(1);
  $('#branchCount').textContent = branches.length;
  renderChart(branches, average);
  renderBranches(branches);
  renderRecent();
  renderTable();
  renderInsights(average, branches);
  $('#branchFilter').innerHTML = '<option value="all">All branches</option>' + branches.map(branch => `<option value="${esc(branch)}">${esc(branch)}</option>`).join('');
}

function renderChart(branches, average) {
  const chartBranches = branches.length ? branches : ['CSE'];
  $('#chart').innerHTML = chartBranches.map(branch => { const values = students.filter(student => student.branch.toUpperCase() === branch); const score = values.length ? values.reduce((sum, student) => sum + Number(student.percentage), 0) / values.length : average; return `<div class="bar-wrap"><div class="bar ${score >= 80 ? 'active' : ''}" style="height:${Math.max(score, 5)}%" title="${score.toFixed(1)}%"></div><label>${esc(branch)}</label></div>`; }).join('');
}

function renderBranches(branches) {
  const maximum = Math.max(...branches.map(branch => students.filter(student => student.branch.toUpperCase() === branch).length), 1);
  $('#branchList').innerHTML = (branches.length ? branches : ['No data']).slice(0, 4).map(branch => { const count = students.filter(student => student.branch.toUpperCase() === branch).length; return `<div class="branch-item"><span>${esc(branch)}</span><div class="progress"><i style="width:${count / maximum * 100}%"></i></div><strong>${count}</strong></div>`; }).join('');
}

function studentRow(student, detailed = false) { const avatarClass = student.roll % 3 === 0 ? 'pink' : student.roll % 2 === 0 ? 'alt' : ''; return detailed ? `<tr><td><div class="student-cell"><span class="student-avatar ${avatarClass}">${initials(esc(student.name))}</span>${esc(student.name)}</div></td><td>#${esc(student.roll)}</td><td>${esc(student.city)}</td><td>${esc(student.branch.toUpperCase())}</td><td>${esc(student.percentage)}%</td><td class="admin-only"><button class="row-action" data-edit="${student.roll}">Edit</button></td></tr>` : `<tr><td><div class="student-cell"><span class="student-avatar ${avatarClass}">${initials(esc(student.name))}</span>${esc(student.name)}</div></td><td>#${esc(student.roll)}</td><td>${esc(student.branch.toUpperCase())}</td><td>${esc(student.percentage)}%</td><td><span class="status">Active</span></td></tr>`; }
function renderRecent() { $('#recentStudents').innerHTML = students.slice().reverse().slice(0, 4).map(student => studentRow(student)).join('') || '<tr><td colspan="5">No student records yet.</td></tr>'; }
function renderTable() { const query = $('#searchInput').value.toLowerCase(); const filter = $('#branchFilter').value; const filtered = students.filter(student => `${student.name} ${student.roll} ${student.branch}`.toLowerCase().includes(query) && (filter === 'all' || student.branch.toUpperCase() === filter)); $('#studentTable').innerHTML = filtered.map(student => studentRow(student, true)).join('') || '<tr><td colspan="6">No matching students found.</td></tr>'; document.querySelectorAll('[data-edit]').forEach(button => button.addEventListener('click', () => openModal(Number(button.dataset.edit)))); }
function renderInsights(average, branches) { const best = students.slice().sort((a,b) => b.percentage - a.percentage)[0]; $('#insightGrid').innerHTML = `<article class="insight-card"><h3>Top performer</h3><strong>${best ? esc(best.name) : '—'}</strong><p>${best ? `${best.percentage}% in ${esc(best.branch.toUpperCase())}` : 'Add students to see insights.'}</p></article><article class="insight-card"><h3>Average percentage</h3><strong>${average.toFixed(1)}%</strong><p>Across ${students.length} student record${students.length === 1 ? '' : 's'}.</p></article><article class="insight-card"><h3>Most represented branch</h3><strong>${branches[0] || '—'}</strong><p>Based on the current student directory.</p></article><article class="insight-card"><h3>Records needing attention</h3><strong>${students.filter(student => student.percentage < 50).length}</strong><p>Students currently below 50%.</p></article>`; }

function openModal(roll = null) { editingRoll = roll; const form = $('#studentForm'); form.reset(); const student = students.find(item => item.roll === roll); $('#modalTitle').textContent = student ? 'Edit student' : 'Add a student'; if (student) Object.entries(student).forEach(([key, value]) => { if (form.elements[key]) form.elements[key].value = value; }); $('#modalBackdrop').classList.remove('hidden'); }
function closeModal() { $('#modalBackdrop').classList.add('hidden'); editingRoll = null; }
function setView(view) { document.querySelectorAll('.view').forEach(item => item.classList.add('hidden')); $(`#${view}View`).classList.remove('hidden'); document.querySelectorAll('.nav-item').forEach(item => item.classList.toggle('active', item.dataset.view === view)); $('#pageTitle').textContent = view === 'overview' ? `Good morning, ${currentRole[0].toUpperCase() + currentRole.slice(1)}.` : view[0].toUpperCase() + view.slice(1); }

document.querySelectorAll('[data-view]').forEach(button => button.addEventListener('click', () => setView(button.dataset.view)));
$('#addStudentButton').addEventListener('click', () => openModal()); $('#addStudentButton2').addEventListener('click', () => openModal()); $('#closeModal').addEventListener('click', closeModal); $('#modalBackdrop').addEventListener('click', event => { if (event.target.id === 'modalBackdrop') closeModal(); }); $('#searchInput').addEventListener('input', renderTable); $('#branchFilter').addEventListener('change', renderTable);
$('#studentForm').addEventListener('submit', event => { event.preventDefault(); const data = Object.fromEntries(new FormData(event.currentTarget)); data.roll = Number(data.roll); data.age = Number(data.age); data.percentage = Number(data.percentage); if (editingRoll && editingRoll !== data.roll && students.some(student => student.roll === data.roll)) return showToast('That roll number already exists.'); const duplicate = students.some(student => student.roll === data.roll && student.roll !== editingRoll); if (duplicate) return showToast('That roll number already exists.'); if (editingRoll) students = students.map(student => student.roll === editingRoll ? data : student); else students.push(data); save(); render(); closeModal(); showToast(editingRoll ? 'Student updated.' : 'Student added.'); });
$('#signOut').addEventListener('click', () => { currentRole = 'admin'; showToast('Signed out of CampusOS.'); });
render();